
#include <M5Unified.h>

#include "my_mrubydef.h"


static void
class_m5_update(mrb_vm *vm, mrb_value *v, int argc)
{
    static unsigned long lastupdate=0;
    unsigned long now=millis();
    if(now - lastupdate > 50) {  // ro prevent accidental double update
        M5.update();
        lastupdate = millis();
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void  // see definition of "enum board_t" in M5GFX/src/lgfx/boards.hpp
class_m5_board(mrb_vm *vm, mrb_value *v, int argc)
{
    SET_INT_RETURN(M5.getBoard());
}


#ifdef USE_TEMPORAL_RANDOM_FUNCTION
static void
class_m5_randomseed(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        randomSeed(val_to_i(vm, v, GET_ARG(1),argc));
    } else {
        randomSeed(analogRead(0));
    }
    SET_TRUE_RETURN();
}

static void
class_m5_random(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>1){
        SET_INT_RETURN(random(val_to_i(vm, v, GET_ARG(1),argc),val_to_i(vm, v, GET_ARG(2),argc)));
    } else if(argc>0){
        SET_INT_RETURN(random(val_to_i(vm, v, GET_ARG(1),argc)));
    } else {
        SET_INT_RETURN(random());
    }
}
#endif // USE_TEMPORAL_RANDOM_FUNCTION

mrbc_value failed_object;

void
false_return(mrb_vm *vm, mrb_value *v, int argc)
{
    SET_FALSE_RETURN();
}
void
true_return(mrb_vm *vm, mrb_value *v, int argc)
{
    SET_TRUE_RETURN();
}


void class_m5_init(){
    mrb_class *class_m5;
    class_m5 = mrbc_define_class(0, "M5", mrbc_class_object);
    mrbc_define_method(0, class_m5, "update", class_m5_update);
    mrbc_define_method(0, class_m5, "board", class_m5_board);
#ifdef USE_TEMPORAL_RANDOM_FUNCTION
    mrbc_define_method(0, class_m5, "randomseed", class_m5_randomseed);
    mrbc_define_method(0, class_m5, "random", class_m5_random);
#endif // USE_TEMPORAL_RANDOM_FUNCTION

    mrb_class *class_failed = mrbc_define_class(0, "FailObject", mrbc_class_object);
    mrbc_define_method(0, class_failed, "method_missing", false_return);
    failed_object = mrbc_instance_new(0, class_failed, 0);
}
