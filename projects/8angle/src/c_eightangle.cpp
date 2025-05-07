#include <M5Unified.h>
#include <M5_ANGLE8.h>

#include "my_mrubydef.h"
#include "c_eightangle.h"

#ifdef USE_EIGHTANGLE

constexpr unsigned int default_eightangle_addr=0x43;

static void class_eightangle_new(mrb_vm *vm, mrb_value *v, int argc)
{
    mrbc_value obj = mrbc_instance_new(vm, v[0].cls, sizeof(M5_ANGLE8*));
    //SET_RETURN(obj);
    v[0] = obj;
    mrbc_instance_call_initialize( vm, v, argc );
}

static void class_eightangle_initialize(mrb_vm *vm, mrb_value *v, int argc)
{
    mrbc_value& obj = v[0];
    unsigned int addr=default_eightangle_addr;
    if(argc>0){
        addr = MRBC_ARG_I(1);
    }
    M5_ANGLE8 *angle8 = new M5_ANGLE8();
    *(M5_ANGLE8**)obj.instance->data = angle8;

    uint8_t retry=20;
    while(retry--){
        if(angle8->begin(addr)){
            return;
        } 
        delay(100);
    }
    mrbc_raise(vm, MRBC_CLASS(RuntimeError), "init failed");
    SET_RETURN(failed_object);
}  

static void
class_eightangle_dswitch(mrb_vm *vm, mrb_value *v, int argc)
{
    M5_ANGLE8 *angle8 = get_checked_data(M5_ANGLE8, vm, v);

    if(angle8->getDigitalInput()){
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void
class_eightangle_set_ledcolor(mrb_vm *vm, mrb_value *v, int argc)
{
    M5_ANGLE8 *angle8 = get_checked_data(M5_ANGLE8, vm, v);
    int ch = MRBC_ARG_I(1);
    uint32_t color = MRBC_ARG_I(2);

    int bright = 25;
    if(argc>2){
        int bright = MRBC_ARG_I(3);
    }
    if(angle8->setLEDColor(ch, color, bright)){
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void
class_eightangle_get_analoginput(mrb_vm *vm, mrb_value *v, int argc)
{
    M5_ANGLE8 *angle8 = get_checked_data(M5_ANGLE8, vm, v);
    int ch = MRBC_ARG_I(1);
    angle8_analog_read_mode_t mode = _8bit;
    if(argc>1){
        mode = (_8bit == MRBC_ARG_I(2)) ? _8bit : _12bit;
    }
    uint16_t val = angle8->getAnalogInput(ch, mode);
    SET_INT_RETURN(val);
}


void class_eightangle_init()
{
    mrb_class *class_eightangle = mrbc_define_class(0, "Angle8Class", mrbc_class_object);
    mrbc_define_method(0, class_eightangle, "new", class_eightangle_new);
    mrbc_define_method(0, class_eightangle, "initialize", class_eightangle_initialize);
    mrbc_define_method(0, class_eightangle, "dswitch", class_eightangle_dswitch);
    mrbc_define_method(0, class_eightangle, "set_led_color", class_eightangle_set_ledcolor);
    mrbc_define_method(0, class_eightangle, "get_analog_input", class_eightangle_get_analoginput);

    mrbc_value angle8obj = mrbc_instance_new(0, class_eightangle, sizeof(M5_ANGLE8*));
    class_eightangle_initialize(0, &angle8obj,  0 );
    mrbc_set_const(mrbc_str_to_symid("Angle8"), &angle8obj);
}

#endif // USE_eightangle