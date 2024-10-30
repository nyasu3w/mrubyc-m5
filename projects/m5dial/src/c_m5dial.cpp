
#include <M5Dial.h>

#include "my_mrubydef.h"
#include "c_m5dial.h"


static void class_m5dial_read(mrb_vm *vm, mrb_value *v, int argc){
    long newpos= M5Dial.Encoder.read();
    SET_INT_RETURN(newpos);
}

static void class_m5dial_write(mrb_vm *vm, mrb_value *v, int argc){
    int pos = val_to_i(vm, v, v[1], argc);
    M5Dial.Encoder.write(pos);
}

static void class_m5dial_read_and_reset(mrb_vm *vm, mrb_value *v, int argc){
    long newpos= M5Dial.Encoder.readAndReset();
    SET_INT_RETURN(newpos);
}

void class_m5dial_init(){
    M5Dial.Encoder.begin();
    mrb_class *class_m5dial = mrbc_define_class(0, "Dial", mrbc_class_object);
    mrbc_define_method(0, class_m5dial, "read", class_m5dial_read);
    mrbc_define_method(0, class_m5dial, "write", class_m5dial_write);
    mrbc_define_method(0, class_m5dial, "read_and_reset", class_m5dial_read_and_reset);
#ifdef USE_M5DIAL_RFID
  // not yet
#endif
}
