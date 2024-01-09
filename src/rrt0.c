/*! @file
  @brief
  Realtime multitask monitor for mruby/c

  <pre>
  Copyright (C) 2015- Kyushu Institute of Technology.
  Copyright (C) 2015- Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.
  </pre>
*/

/***** Feature test switches ************************************************/
/***** System headers *******************************************************/
//@cond
#include "vm_config.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
//@endcond


/***** Local headers ********************************************************/
#include "alloc.h"
#include "load.h"
#include "class.h"
#include "global.h"
#include "symbol.h"
#include "vm.h"
#include "console.h"
#include "c_string.h"
#include "c_array.h"
#include "rrt0.h"
#include "hal_selector.h"


/***** Macros ***************************************************************/
#ifndef MRBC_SCHEDULER_EXIT
#define MRBC_SCHEDULER_EXIT 0
#endif

#define VM2TCB(p) ((mrbc_tcb *)((uint8_t *)p - offsetof(mrbc_tcb, vm)))
#define MRBC_MUTEX_TRACE(...) ((void)0)


/***** Typedefs *************************************************************/
/***** Function prototypes **************************************************/
/***** Local variables ******************************************************/
static mrbc_tcb *q_dormant_;
static mrbc_tcb *q_ready_;
static mrbc_tcb *q_waiting_;
static mrbc_tcb *q_suspended_;
static volatile uint32_t tick_;


/***** Global variables *****************************************************/
/***** Signal catching functions ********************************************/
/***** Functions ************************************************************/

//================================================================
/*! Insert task(TCB) to task queue

  @param  p_tcb	Pointer to target TCB

  Put the task (TCB) into a queue by each state.
  TCB must be free. (must not be in another queue)
  The queue is sorted in priority_preemption order.
  If the same priority_preemption value is in the TCB and queue,
  it will be inserted at the end of the same value in queue.
*/
static void q_insert_task(mrbc_tcb *p_tcb)
{
  mrbc_tcb **pp_q;

  switch( p_tcb->state ) {
  case TASKSTATE_DORMANT: pp_q   = &q_dormant_; break;
  case TASKSTATE_READY:
  case TASKSTATE_RUNNING: pp_q   = &q_ready_; break;
  case TASKSTATE_WAITING: pp_q   = &q_waiting_; break;
  case TASKSTATE_SUSPENDED: pp_q = &q_suspended_; break;
  default:
    assert(!"Wrong task state.");
    return;
  }

  // case insert on top.
  if((*pp_q == NULL) ||
     (p_tcb->priority_preemption < (*pp_q)->priority_preemption)) {
    p_tcb->next = *pp_q;
    *pp_q       = p_tcb;
    return;
  }

  // find insert point in sorted linked list.
  mrbc_tcb *p = *pp_q;
  while( p->next != NULL ) {
    if( p_tcb->priority_preemption < p->next->priority_preemption ) break;
    p = p->next;
  }

  // insert tcb to queue.
  p_tcb->next = p->next;
  p->next     = p_tcb;
}


//================================================================
/*! Delete task(TCB) from task queue

  @param  p_tcb	Pointer to target TCB
*/
static void q_delete_task(mrbc_tcb *p_tcb)
{
  mrbc_tcb **pp_q;

  switch( p_tcb->state ) {
  case TASKSTATE_DORMANT: pp_q   = &q_dormant_; break;
  case TASKSTATE_READY:
  case TASKSTATE_RUNNING: pp_q   = &q_ready_; break;
  case TASKSTATE_WAITING: pp_q   = &q_waiting_; break;
  case TASKSTATE_SUSPENDED: pp_q = &q_suspended_; break;
  default:
    assert(!"Wrong task state.");
    return;
  }
  assert( *pp_q );

  if( *pp_q == p_tcb ) {
    *pp_q       = p_tcb->next;
    p_tcb->next = NULL;
    return;
  }

  mrbc_tcb *p = *pp_q;
  while( p ) {
    if( p->next == p_tcb ) {
      p->next     = p_tcb->next;
      p_tcb->next = NULL;
      return;
    }

    p = p->next;
  }

  assert(!"Not found target task in queue.");
}


//================================================================
/*! Tick timer interrupt handler.

*/
void mrbc_tick(void)
{
  mrbc_tcb *tcb;
  int flag_preemption = 0;

  tick_++;

  // Decrease the time slice value for running tasks.
  tcb = q_ready_;
  if((tcb != NULL) &&
     (tcb->state == TASKSTATE_RUNNING) &&
     (tcb->timeslice > 0)) {
    tcb->timeslice--;
    if( tcb->timeslice == 0 ) tcb->vm.flag_preemption = 1;
  }

  // Find a wake up task in waiting task queue.
  tcb = q_waiting_;
  while( tcb != NULL ) {
    mrbc_tcb *t = tcb;
    tcb = tcb->next;

    if( t->reason == TASKREASON_SLEEP &&
        (int32_t)(t->wakeup_tick - tick_) <= 0 ) {
      q_delete_task(t);
      t->state     = TASKSTATE_READY;
      t->reason    = 0;
      t->timeslice = MRBC_TIMESLICE_TICK_COUNT;
      q_insert_task(t);
      flag_preemption = 1;
    }
  }

  if( flag_preemption ) {
    tcb = q_ready_;
    while( tcb != NULL ) {
      if( tcb->state == TASKSTATE_RUNNING ) tcb->vm.flag_preemption = 1;
      tcb = tcb->next;
    }
  }
}


//================================================================
/*! create (allocate) TCB.

  @param  regs_size	num of allocated registers.
  @param  task_state	task initial state.
  @param  priority	task priority.
  @return pointer to TCB or NULL.

<b>Code example</b>
@code
  //  If you want specify default value, see below.
  //    regs_size:  MAX_REGS_SIZE (in vm_config.h)
  //    task_state: MRBC_TASK_DEFAULT_STATE
  //    priority:   MRBC_TASK_DEFAULT_PRIORITY
  mrbc_tcb *tcb;
  tcb = mrbc_tcb_new( MAX_REGS_SIZE, MRBC_TASK_DEFAULT_STATE, MRBC_TASK_DEFAULT_PRIORITY );
  mrbc_create_task( byte_code, tcb );
@endcode
*/
mrbc_tcb * mrbc_tcb_new( int regs_size, enum MrbcTaskState task_state, int priority )
{
  mrbc_tcb *tcb;

  tcb = mrbc_raw_alloc( sizeof(mrbc_tcb) + sizeof(mrbc_value) * regs_size );
  if( !tcb ) return NULL;	// ENOMEM

  memset(tcb, 0, sizeof(mrbc_tcb));
#if defined(MRBC_DEBUG)
  memcpy( tcb->type, "TCB", 4 );
#endif
  tcb->priority = priority;
  tcb->state = task_state;
  tcb->vm.regs_size = regs_size;

  return tcb;
}


//================================================================
/*! specify running VM code.

  @param  byte_code	pointer to VM byte code.
  @param  tcb		Task control block with parameter, or NULL.
  @return Pointer to mrbc_tcb or NULL.
*/
mrbc_tcb * mrbc_create_task(const void *byte_code, mrbc_tcb *tcb)
{
  if( !tcb ) tcb = mrbc_tcb_new( MAX_REGS_SIZE, MRBC_TASK_DEFAULT_STATE, MRBC_TASK_DEFAULT_PRIORITY );
  if( !tcb ) return NULL;	// ENOMEM

  tcb->timeslice = MRBC_TIMESLICE_TICK_COUNT;
  tcb->priority_preemption = tcb->priority;

  // assign VM ID
  if( mrbc_vm_open( &tcb->vm ) == NULL ) {
    mrbc_printf("Error: Can't assign VM-ID.\n");
    return NULL;
  }

  if( mrbc_load_mrb(&tcb->vm, byte_code) != 0 ) {
    mrbc_print_vm_exception( &tcb->vm );
    mrbc_vm_close( &tcb->vm );
    return NULL;
  }
  if( tcb->state != TASKSTATE_DORMANT ) {
    mrbc_vm_begin( &tcb->vm );
  }

  hal_disable_irq();
  q_insert_task(tcb);
  hal_enable_irq();

  return tcb;
}


//================================================================
/*! set the task name.

  @param  tcb	target task.
  @param  name	task name
*/
void mrbc_set_task_name(mrbc_tcb *tcb, const char *name)
{
  strncpy( tcb->name, name, MRBC_TASK_NAME_LEN );
}


//================================================================
/*! find task by name

  @param  name		task name
  @return pointer to mrbc_tcb or NULL
*/
mrbc_tcb * mrbc_find_task(const char *name)
{
  mrbc_tcb *tcb;

  hal_disable_irq();

  for( tcb = q_ready_; tcb != NULL; tcb = tcb->next ) {
    if( strcmp( tcb->name, name ) == 0 ) goto RETURN_TCB;
  }
  for( tcb = q_waiting_; tcb != NULL; tcb = tcb->next ) {
    if( strcmp( tcb->name, name ) == 0 ) goto RETURN_TCB;
  }
  for( tcb = q_suspended_; tcb != NULL; tcb = tcb->next ) {
    if( strcmp( tcb->name, name ) == 0 ) goto RETURN_TCB;
  }
  for( tcb = q_dormant_; tcb != NULL; tcb = tcb->next ) {
    if( strcmp( tcb->name, name ) == 0 ) goto RETURN_TCB;
  }

 RETURN_TCB:
  hal_enable_irq();
  return tcb;
}


//================================================================
/*! Start execution of dormant task.

  @param  tcb	Task control block with parameter, or NULL.
  @retval int	zero / no error.
*/
int mrbc_start_task(mrbc_tcb *tcb)
{
  if( tcb->state != TASKSTATE_DORMANT ) return -1;
  tcb->timeslice           = MRBC_TIMESLICE_TICK_COUNT;
  tcb->priority_preemption = tcb->priority;
  mrbc_vm_begin(&tcb->vm);

  hal_disable_irq();

  mrbc_tcb *t = q_ready_;
  while( t != NULL ) {
    if( t->state == TASKSTATE_RUNNING ) t->vm.flag_preemption = 1;
    t = t->next;
  }

  q_delete_task(tcb);
  tcb->state = TASKSTATE_READY;
  q_insert_task(tcb);
  hal_enable_irq();

  return 0;
}


//================================================================
/*! execute

*/
int mrbc_run(void)
{
  int ret = 1;

#if MRBC_SCHEDULER_EXIT
  if( q_ready_ == NULL && q_waiting_ == NULL && q_suspended_ == NULL ) return 0;
#endif

  while( 1 ) {
    mrbc_tcb *tcb = q_ready_;
    if( tcb == NULL ) {		// no task to run.
      hal_idle_cpu();
      continue;
    }

    tcb->state = TASKSTATE_RUNNING;	// to execute.
    int res = 0;

#ifndef MRBC_NO_TIMER
    tcb->vm.flag_preemption = 0;
    res = mrbc_vm_run(&tcb->vm);

#else
    while( tcb->timeslice > 0 ) {
      tcb->vm.flag_preemption = 1;
      res = mrbc_vm_run(&tcb->vm);
      tcb->timeslice--;
      if( res != 0 ) break;
      if( tcb->state != TASKSTATE_RUNNING ) break;
    }
    mrbc_tick();
#endif /* ifndef MRBC_NO_TIMER */

    // did the task done?
    if( res != 0 ) {
      hal_disable_irq();
      q_delete_task(tcb);
      tcb->state = TASKSTATE_DORMANT;
      q_insert_task(tcb);
      hal_enable_irq();
      if( tcb->vm.flag_permanence == 0 ) mrbc_vm_end(&tcb->vm);
      if( res != 1 ) ret = res;

#if MRBC_SCHEDULER_EXIT
      if( q_ready_ == NULL && q_waiting_ == NULL && q_suspended_ == NULL ) break;
#endif
      continue;
    }

    // switch task.
    hal_disable_irq();
    if( tcb->state == TASKSTATE_RUNNING ) {
      tcb->state = TASKSTATE_READY;

      if( tcb->timeslice == 0 ) {
        q_delete_task(tcb);
        tcb->timeslice = MRBC_TIMESLICE_TICK_COUNT;
        q_insert_task(tcb); // insert task on queue last.
      }
    }
    hal_enable_irq();
  }

  return ret;
}


//================================================================
/*! sleep for a specified number of milliseconds.

  @param  tcb	target task.
  @param  ms	sleep milliseconds.
*/
void mrbc_sleep_ms(mrbc_tcb *tcb, uint32_t ms)
{
  hal_disable_irq();
  q_delete_task(tcb);
  tcb->timeslice   = 0;
  tcb->state       = TASKSTATE_WAITING;
  tcb->reason      = TASKREASON_SLEEP;
  tcb->wakeup_tick = tick_ + (ms / MRBC_TICK_UNIT) + 1;
  if( ms % MRBC_TICK_UNIT ) tcb->wakeup_tick++;
  q_insert_task(tcb);
  hal_enable_irq();

  tcb->vm.flag_preemption = 1;
}


//================================================================
/*! Relinquish control to other tasks.

  @param  tcb	target task.
*/
void mrbc_relinquish(mrbc_tcb *tcb)
{
  tcb->timeslice          = 0;
  tcb->vm.flag_preemption = 1;
}


//================================================================
/*! change task priority.

  @param  tcb		target task.
  @param  priority	priority value. between 1 and 255.
*/
void mrbc_change_priority(mrbc_tcb *tcb, int priority)
{
  tcb->priority            = priority;
  tcb->priority_preemption = priority;
  tcb->timeslice           = 0;
  tcb->vm.flag_preemption  = 1;
}


//================================================================
/*! suspend the task.

  @param  tcb		target task.
*/
void mrbc_suspend_task(mrbc_tcb *tcb)
{
  if( tcb->state != TASKSTATE_READY &&
      tcb->state != TASKSTATE_RUNNING &&
      tcb->state != TASKSTATE_WAITING ) return;

  hal_disable_irq();
  q_delete_task(tcb);
  tcb->state = TASKSTATE_SUSPENDED;
  q_insert_task(tcb);
  hal_enable_irq();

  tcb->vm.flag_preemption = 1;
}


//================================================================
/*! resume the task

  @param  tcb		target task.
*/
void mrbc_resume_task(mrbc_tcb *tcb)
{
  if( tcb->state != TASKSTATE_SUSPENDED ) return;

  int flag_to_ready_state = (tcb->reason == 0);

  hal_disable_irq();

  if( flag_to_ready_state ) {
    mrbc_tcb *t = q_ready_;
    while( t != NULL ) {
      if( t->state == TASKSTATE_RUNNING ) t->vm.flag_preemption = 1;
      t = t->next;
    }
  }

  q_delete_task(tcb);
  tcb->state = flag_to_ready_state ? TASKSTATE_READY : TASKSTATE_WAITING;
  q_insert_task(tcb);

  hal_enable_irq();
}


//================================================================
/*! mutex initialize

  @param  mutex		pointer to mrbc_mutex or NULL.
*/
mrbc_mutex * mrbc_mutex_init( mrbc_mutex *mutex )
{
  if( mutex == NULL ) {
    mutex = mrbc_raw_alloc( sizeof(mrbc_mutex) );
    if( mutex == NULL ) return NULL;	// ENOMEM
  }

  static const mrbc_mutex init_val = MRBC_MUTEX_INITIALIZER;
  *mutex = init_val;

  return mutex;
}


//================================================================
/*! mutex lock

  @param  mutex		pointer to mutex.
  @param  tcb		pointer to TCB.
*/
int mrbc_mutex_lock( mrbc_mutex *mutex, mrbc_tcb *tcb )
{
  MRBC_MUTEX_TRACE("mutex lock / MUTEX: %p TCB: %p",  mutex, tcb );

  int ret = 0;
  hal_disable_irq();

  // Try lock mutex;
  if( mutex->lock == 0 ) {      // a future does use TAS?
    mutex->lock = 1;
    mutex->tcb = tcb;
    MRBC_MUTEX_TRACE("  lock OK\n" );
    goto DONE;
  }
  MRBC_MUTEX_TRACE("  lock FAIL\n" );

  // Can't lock mutex
  // check recursive lock.
  if( mutex->tcb == tcb ) {
    ret = 1;
    goto DONE;
  }

  // To WAITING state.
  q_delete_task(tcb);
  tcb->state  = TASKSTATE_WAITING;
  tcb->reason = TASKREASON_MUTEX;
  tcb->mutex = mutex;
  q_insert_task(tcb);
  tcb->vm.flag_preemption = 1;

 DONE:
  hal_enable_irq();

  return ret;
}


//================================================================
/*! mutex unlock

  @param  mutex		pointer to mutex.
  @param  tcb		pointer to TCB.
*/
int mrbc_mutex_unlock( mrbc_mutex *mutex, mrbc_tcb *tcb )
{
  MRBC_MUTEX_TRACE("mutex unlock / MUTEX: %p TCB: %p\n",  mutex, tcb );

  // check some parameters.
  if( !mutex->lock ) return 1;
  if( mutex->tcb != tcb ) return 2;

  hal_disable_irq();

  // wakeup ONE waiting task if exist.
  mrbc_tcb *tcb1;
  for( tcb1 = q_waiting_; tcb1 != NULL; tcb1 = tcb1->next ) {
    if( tcb1->reason == TASKREASON_MUTEX && tcb1->mutex == mutex ) break;
  }
  if( tcb1 ) {
    MRBC_MUTEX_TRACE("SW1: TCB: %p\n", tcb1 );
    mutex->tcb = tcb1;

    q_delete_task(tcb1);
    tcb1->state = TASKSTATE_READY;
    tcb1->reason = 0;
    q_insert_task(tcb1);

    for( tcb1 = q_ready_; tcb1 != NULL; tcb1 = tcb1->next ) {
      if( tcb1->state == TASKSTATE_RUNNING ) tcb1->vm.flag_preemption = 1;
    }
    goto DONE;
  }

  // find ONE mutex locked task in suspended queue.
  for( tcb1 = q_suspended_; tcb1 != NULL; tcb1 = tcb1->next ) {
    if( tcb1->reason == TASKREASON_MUTEX && tcb1->mutex == mutex ) break;
  }
  if( tcb1 ) {
    MRBC_MUTEX_TRACE("SW2: TCB: %p\n", tcb1 );
    mutex->tcb = tcb1;
    tcb1->reason = 0;
    goto DONE;
  }

  // other case, unlock mutex
  MRBC_MUTEX_TRACE("mutex unlock all.\n" );
  mutex->lock = 0;
  mutex->tcb = 0;

 DONE:
  hal_enable_irq();

  return 0;
}


//================================================================
/*! mutex trylock

  @param  mutex		pointer to mutex.
  @param  tcb		pointer to TCB.
*/
int mrbc_mutex_trylock( mrbc_mutex *mutex, mrbc_tcb *tcb )
{
  MRBC_MUTEX_TRACE("mutex try lock / MUTEX: %p TCB: %p",  mutex, tcb );

  int ret;
  hal_disable_irq();

  if( mutex->lock == 0 ) {
    mutex->lock = 1;
    mutex->tcb = tcb;
    ret = 0;
    MRBC_MUTEX_TRACE("  trylock OK\n" );
  }
  else {
    MRBC_MUTEX_TRACE("  trylock FAIL\n" );
    ret = 1;
  }

  hal_enable_irq();
  return ret;
}


//================================================================
/*! clenaup all resources.

*/
void mrbc_cleanup(void)
{
  mrbc_cleanup_vm();
  mrbc_cleanup_symbol();
  mrbc_cleanup_alloc();

  q_dormant_ = 0;
  q_ready_ = 0;
  q_waiting_ = 0;
  q_suspended_ = 0;
}


//================================================================
/*! (method) sleep for a specified number of seconds (CRuby compatible)

*/
static void c_sleep(mrbc_vm *vm, mrbc_value v[], int argc)
{
  mrbc_tcb *tcb = VM2TCB(vm);

  if( argc == 0 ) {
    mrbc_suspend_task(tcb);
    return;
  }

  switch( mrbc_type(v[1]) ) {
  case MRBC_TT_INTEGER:
  {
    mrbc_int_t sec;
    sec = mrbc_integer(v[1]);
    SET_INT_RETURN(sec);
    mrbc_sleep_ms(tcb, sec * 1000);
    break;
  }

#if MRBC_USE_FLOAT
  case MRBC_TT_FLOAT:
  {
    mrbc_float_t sec;
    sec = mrbc_float(v[1]);
    SET_INT_RETURN(sec);
    mrbc_sleep_ms(tcb, (mrbc_int_t)(sec * 1000));
    break;
  }
#endif

  default:
    break;
  }
}


//================================================================
/*! (method) sleep for a specified number of milliseconds.

*/
static void c_sleep_ms(mrbc_vm *vm, mrbc_value v[], int argc)
{
  mrbc_tcb *tcb = VM2TCB(vm);

  mrbc_int_t sec = mrbc_integer(v[1]);
  SET_INT_RETURN(sec);
  mrbc_sleep_ms(tcb, sec);
}


//================================================================
/*! (method) relinquish control to other tasks. (BETA)

*/
static void c_relinquish(mrbc_vm *vm, mrbc_value v[], int argc)
{
  mrbc_tcb *tcb = VM2TCB(vm);

  mrbc_relinquish(tcb);
}


//================================================================
/*! (method) change task priority.

*/
static void c_change_priority(mrbc_vm *vm, mrbc_value v[], int argc)
{
  mrbc_tcb *tcb = VM2TCB(vm);

  mrbc_change_priority(tcb, mrbc_integer(v[1]));
}


//================================================================
/*! (method) suspend the task. (BETA)

*/
static void c_suspend_task(mrbc_vm *vm, mrbc_value v[], int argc)
{
  if( argc == 0 ) {
    mrbc_tcb *tcb = VM2TCB(vm);
    mrbc_suspend_task(tcb);	// suspend self.
    return;
  }

  if( mrbc_type(v[1]) != MRBC_TT_HANDLE ) return;	// error.
  mrbc_suspend_task( (mrbc_tcb *)(v[1].handle) );
}


//================================================================
/*! (method) resume the task (BETA)

*/
static void c_resume_task(mrbc_vm *vm, mrbc_value v[], int argc)
{
  if( mrbc_type(v[1]) != MRBC_TT_HANDLE ) return;	// error.
  mrbc_resume_task( (mrbc_tcb *)(v[1].handle) );
}


//================================================================
/*! (method) get the TCB (BETA)

*/
static void c_get_tcb(mrbc_vm *vm, mrbc_value v[], int argc)
{
  mrbc_tcb *tcb = VM2TCB(vm);

  mrbc_value value = {.tt = MRBC_TT_HANDLE};
  value.handle = tcb;

  SET_RETURN( value );
}



/*
  Task class
*/
//================================================================
/*! (method) Task constructor

  task = Task.new()                 # returns current task.
  task = Task.new("OtherTaskName")
*/
static void c_task_new(mrbc_vm *vm, mrbc_value v[], int argc)
{
  *v = mrbc_instance_new(vm, v->cls, sizeof(mrbc_tcb *));

  if( argc == 0 ) {
    *(mrbc_tcb **)v->instance->data = VM2TCB(vm);
    return;
  }

  if( v[1].tt == MRBC_TT_STRING ) {
    mrbc_tcb *tcb = mrbc_find_task( mrbc_string_cstr( &v[1] ) );
    if( tcb ) {
      *(mrbc_tcb **)v->instance->data = tcb;
      return;
    }
  }

  mrbc_raise( vm, MRBC_CLASS(ArgumentError), 0 );
}


//================================================================
/*! (method) task list

  Task.list() -> Array[Task]
*/
static void c_task_list(mrbc_vm *vm, mrbc_value v[], int argc)
{
  mrbc_tcb* qlist[] = {q_dormant_, q_ready_, q_waiting_, q_suspended_};
  mrbc_value ret = mrbc_array_new(vm, 1);

  hal_disable_irq();

  for( int i = 0; i < sizeof(qlist)/sizeof(mrbc_tcb*); i++ ) {
    for( mrbc_tcb *tcb = qlist[i]; tcb != NULL; tcb = tcb->next ) {
      mrbc_value task = mrbc_instance_new(vm, v->cls, sizeof(mrbc_tcb *));
      *(mrbc_tcb **)task.instance->data = VM2TCB(vm);
      mrbc_array_push( &ret, &task );
    }
  }

  hal_enable_irq();

  SET_RETURN(ret);
}


//================================================================
/*! (method) task name list

  Task.list() -> Array[String]
*/
static void c_task_name_list(mrbc_vm *vm, mrbc_value v[], int argc)
{
  mrbc_tcb* qlist[] = {q_dormant_, q_ready_, q_waiting_, q_suspended_};
  mrbc_value ret = mrbc_array_new(vm, 1);

  hal_disable_irq();

  for( int i = 0; i < sizeof(qlist)/sizeof(mrbc_tcb*); i++ ) {
    for( mrbc_tcb *tcb = qlist[i]; tcb != NULL; tcb = tcb->next ) {
      mrbc_value s = mrbc_string_new_cstr(vm, tcb->name);
      mrbc_array_push( &ret, &s );
    }
  }

  hal_enable_irq();

  SET_RETURN(ret);
}


//================================================================
/*! (method) name setter.
*/
static void c_task_set_name(mrbc_vm *vm, mrbc_value v[], int argc)
{
  if( v[1].tt != MRBC_TT_STRING ) {
    mrbc_raise( vm, MRBC_CLASS(ArgumentError), 0 );
    return;
  }

  mrbc_tcb *tcb;

  if( v[0].tt == MRBC_TT_CLASS ) {
    tcb = VM2TCB(vm);
  } else {
    tcb = *(mrbc_tcb **)v[0].instance->data;
  }
  mrbc_set_task_name( tcb, mrbc_string_cstr(&v[1]) );
}


//================================================================
/*! (method) name getter
*/
static void c_task_name(mrbc_vm *vm, mrbc_value v[], int argc)
{
  mrbc_value ret;

  if( v[0].tt == MRBC_TT_CLASS ) {
    ret = mrbc_string_new_cstr( vm, VM2TCB(vm)->name );
  } else {
    mrbc_tcb *tcb = *(mrbc_tcb **)v[0].instance->data;
    ret = mrbc_string_new_cstr(vm, tcb->name );
  }

  SET_RETURN(ret);
}


//================================================================
/*! (method) suspend task
*/
static void c_task_suspend(mrbc_vm *vm, mrbc_value v[], int argc)
{
  mrbc_tcb *tcb;

  if( v[0].tt == MRBC_TT_CLASS ) {
    tcb = VM2TCB(vm);
  } else {
    tcb = *(mrbc_tcb **)v[0].instance->data;
  }

  mrbc_suspend_task(tcb);
}


//================================================================
/*! (method) resume task
*/
static void c_task_resume(mrbc_vm *vm, mrbc_value v[], int argc)
{
  if( v[0].tt == MRBC_TT_CLASS ) return;

  mrbc_tcb *tcb = *(mrbc_tcb **)v[0].instance->data;

  mrbc_resume_task(tcb);
}



/*
  Mutex class
*/
//================================================================
/*! (method) mutex constructor

*/
static void c_mutex_new(mrbc_vm *vm, mrbc_value v[], int argc)
{
  *v = mrbc_instance_new(vm, v->cls, sizeof(mrbc_mutex));
  if( !v->instance ) return;

  mrbc_mutex_init( (mrbc_mutex *)(v->instance->data) );
}


//================================================================
/*! (method) mutex lock

*/
static void c_mutex_lock(mrbc_vm *vm, mrbc_value v[], int argc)
{
  int r = mrbc_mutex_lock( (mrbc_mutex *)v->instance->data, VM2TCB(vm) );
  if( r == 0 ) return;  // return self

  // raise ThreadError
  assert(!"Mutex recursive lock.");
}


//================================================================
/*! (method) mutex unlock

*/
static void c_mutex_unlock(mrbc_vm *vm, mrbc_value v[], int argc)
{
  int r = mrbc_mutex_unlock( (mrbc_mutex *)v->instance->data, VM2TCB(vm) );
  if( r == 0 ) return;  // return self

  // raise ThreadError
  assert(!"Mutex unlock error. not owner or not locked.");
}


//================================================================
/*! (method) mutex trylock

*/
static void c_mutex_trylock(mrbc_vm *vm, mrbc_value v[], int argc)
{
  int r = mrbc_mutex_trylock( (mrbc_mutex *)v->instance->data, VM2TCB(vm) );
  SET_BOOL_RETURN( r == 0 );
}


//================================================================
/*! (method) mutex locked?

*/
static void c_mutex_locked(mrbc_vm *vm, mrbc_value v[], int argc)
{
  mrbc_mutex *mutex = (mrbc_mutex *)v->instance->data;
  SET_BOOL_RETURN( mutex->lock != 0 );
}


//================================================================
/*! (method) mutex owned?

*/
static void c_mutex_owned(mrbc_vm *vm, mrbc_value v[], int argc)
{
  mrbc_mutex *mutex = (mrbc_mutex *)v->instance->data;
  SET_BOOL_RETURN( mutex->lock != 0 && mutex->tcb == VM2TCB(vm) );
}


//================================================================
/*! (method) get tick counter
*/
static void c_vm_tick(mrbc_vm *vm, mrbc_value v[], int argc)
{
  SET_INT_RETURN(tick_);
}



//================================================================
/*! initialize

  @param  heap_ptr	heap memory buffer.
  @param  size		its size.
*/
void mrbc_init(void *heap_ptr, unsigned int size)
{
  hal_init();
  mrbc_init_alloc(heap_ptr, size);
  mrbc_init_global();
  mrbc_init_class();

  mrbc_define_method(0, mrbc_class_object, "sleep",           c_sleep);
  mrbc_define_method(0, mrbc_class_object, "sleep_ms",        c_sleep_ms);
  mrbc_define_method(0, mrbc_class_object, "relinquish",      c_relinquish);
  mrbc_define_method(0, mrbc_class_object, "change_priority", c_change_priority);
  mrbc_define_method(0, mrbc_class_object, "suspend_task",    c_suspend_task);
  mrbc_define_method(0, mrbc_class_object, "resume_task",     c_resume_task);
  mrbc_define_method(0, mrbc_class_object, "get_tcb",	      c_get_tcb);

  mrbc_class *c_task;
  c_task = mrbc_define_class(0, "Task", 0);
  mrbc_define_method(0, c_task, "new", c_task_new);
  mrbc_define_method(0, c_task, "list", c_task_list);
  mrbc_define_method(0, c_task, "name_list", c_task_name_list);
  mrbc_define_method(0, c_task, "name=", c_task_set_name);
  mrbc_define_method(0, c_task, "name", c_task_name);
  mrbc_define_method(0, c_task, "suspend", c_task_suspend);
  mrbc_define_method(0, c_task, "resume", c_task_resume);


  mrbc_class *c_mutex;
  c_mutex = mrbc_define_class(0, "Mutex", 0);
  mrbc_define_method(0, c_mutex, "new", c_mutex_new);
  mrbc_define_method(0, c_mutex, "lock", c_mutex_lock);
  mrbc_define_method(0, c_mutex, "unlock", c_mutex_unlock);
  mrbc_define_method(0, c_mutex, "try_lock", c_mutex_trylock);
  mrbc_define_method(0, c_mutex, "locked?", c_mutex_locked);
  mrbc_define_method(0, c_mutex, "owned?", c_mutex_owned);

  mrbc_class *c_vm;
  c_vm = mrbc_define_class(0, "VM", mrbc_class_object);
  mrbc_define_method(0, c_vm, "tick", c_vm_tick);
}



#ifdef MRBC_DEBUG

//================================================================
/*! DEBUG print queue

 */
void pq(const mrbc_tcb *p_tcb)
{
  const mrbc_tcb *p;

  if( p_tcb == NULL ) return;

  // TCB address
  p = p_tcb;
  while( p != NULL ) {
#if defined(UINTPTR_MAX)
    mrbc_printf("%08x  ", (uint32_t)(uintptr_t)p);
#else
    mrbc_printf("%08x  ", (uint32_t)p);
#endif
    p = p->next;
  }
  mrbc_printf("\n");

  // name
  p = p_tcb;
  while( p != NULL ) {
    mrbc_printf("%-9.9s ", p->name[0] ? p->name : "(noname)" );
    p = p->next;
  }
  mrbc_printf("\n");

  // next pointer.
  p = p_tcb;
  while( p != NULL ) {
#if defined(UINTPTR_MAX)
    mrbc_printf(" nx:%04x  ", (uint16_t)(uintptr_t)p->next);
#else
    mrbc_printf(" nx:%04x  ", (uint16_t)p->next);
#endif
    p = p->next;
  }
  mrbc_printf("\n");

  // task priority.
  p = p_tcb;
  while( p != NULL ) {
    mrbc_printf(" pri:%3d  ", p->priority_preemption);
    p = p->next;
  }
  mrbc_printf("\n");

  // state
  //  st:SsRr
  //     ^ suspended -> S:suspended
  //      ^ waiting  -> s:sleep m:mutex
  //       ^ running -> R:running
  //        ^ ready  -> r:ready
  p = p_tcb;
  while( p != NULL ) {
    mrbc_printf(" st:%c%c%c%c  ",
                (p->state & TASKSTATE_SUSPENDED)?'S':'-',
                ("-sm"[p->reason]),
                (p->state &(TASKSTATE_RUNNING & ~TASKSTATE_READY))?'R':'-',
                (p->state & TASKSTATE_READY)?'r':'-' );
    p = p->next;
  }
  mrbc_printf("\n");

  // timeslice
  p = p_tcb;
  while( p != NULL ) {
    mrbc_printf(" tmsl:%2d  ", p->timeslice);
    p = p->next;
  }
  mrbc_printf("\n");
}


void pqall(void)
{
  mrbc_printf("<<<<< DORMANT >>>>>\n");   pq(q_dormant_);
  mrbc_printf("<<<<< READY >>>>>\n");     pq(q_ready_);
  mrbc_printf("<<<<< WAITING >>>>>\n");   pq(q_waiting_);
  mrbc_printf("<<<<< SUSPENDED >>>>>\n"); pq(q_suspended_);
}
#endif
