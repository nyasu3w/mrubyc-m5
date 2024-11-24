#include <M5Unified.h>
#include "my_mrubydef.h"
#include "c_mdisplays.h"

#ifdef USE_MULTIDISPLAY

static void
class_m5_display_counts(mrb_vm *vm, mrb_value *v, int argc)
{
    SET_INT_RETURN(M5.getDisplayCount());
}

int current_deviceno() {
    for(int i=0;i<M5.getDisplayCount();i++){
        if(&M5.Display == &M5.getDisplay(i)){
            return i;
        }
    }
    return -1;
}

static void
class_display_get_deviceno(mrb_vm *vm, mrb_value *v, int argc)
{
    int no = current_deviceno();
    if(0<=no){
        SET_INT_RETURN(no);
    } else {
        SET_NIL_RETURN();
    }
}

static void
class_display_change_device(mrb_vm *vm, mrb_value *v, int argc)
{
    mrbc_value ret;
    int f_devno=current_deviceno();

    if(argc>0){
        int no = val_to_i(vm, v, GET_ARG(1),argc);
        if(0<=no && no<M5.getDisplayCount()){
            M5.setPrimaryDisplay(no);
            SET_TRUE_RETURN();
        } else {
            SET_FALSE_RETURN();
        }
    } else {
        SET_FALSE_RETURN();
    }

    if(GET_ARG(argc+1).tt==MRBC_TT_PROC) {
        mrbc_method method;

        /*  It is not clear how to yield for the block */

        M5.setPrimaryDisplay(f_devno);
        SET_RETURN(ret);
    }
}

static void
class_display_get_device_type(mrb_vm *vm, mrb_value *v, int argc)
{

    if(argc>0){
        int no = val_to_i(vm, v, GET_ARG(1),argc);
        if(0<=no && no<M5.getDisplayCount()){
            SET_INT_RETURN((int) M5.getDisplay(no).getBoard());
            return;
        }
    } else {
            SET_INT_RETURN((int)M5.Display.getBoard());
    }
}


void mdisplays_init()
{
    mrbc_class *class_display = mrbc_get_class_by_name("Display");
    mrbc_class *class_m5 = mrbc_get_class_by_name("M5");

    mrbc_define_method(0, class_m5, "display_counts", class_m5_display_counts);

    mrbc_define_method(0, class_display, "change", class_display_change_device);
    mrbc_define_method(0, class_display, "get_index", class_display_get_deviceno);
    mrbc_define_method(0, class_display, "device_type", class_display_get_device_type);

}

#endif

