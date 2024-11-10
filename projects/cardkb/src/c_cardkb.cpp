#include <M5Unified.h>
#include <Wire.h>

#include "my_mrubydef.h"
#include "c_cardkb.h"

#ifdef USE_CARDKB

constexpr unsigned int default_cardkb_addr=0x5f;

static void class_cardkb_new(mrb_vm *vm, mrb_value *v, int argc)
{
    v[0] = mrbc_instance_new(vm, v[0].cls, sizeof(int));
    mrbc_instance_call_initialize( vm, v, argc );
}  

static void
class_cardkb_initialize(mrb_vm *vm, mrb_value *v, int argc)
{
    unsigned int addr=0x5f;
    if(argc>0){
        addr = val_to_i(vm, v, v[1],argc);
    }
    *v->instance->data = addr;
}

static void
class_cardkb_getaddr(mrb_vm *vm, mrb_value *v, int argc)
{
    SET_INT_RETURN(*v->instance->data);
}

static void
class_cardkb_getch(mrb_vm *vm, mrb_value *v, int argc)
{
    Wire.requestFrom((int)*v->instance->data, 1);
    uint8_t d = 0;
    while(Wire.available()){
        d = Wire.read();
    }
    SET_INT_RETURN(d);
}

static bool cardkb_availability(uint8_t addr)
{
    Wire.beginTransmission(addr);
    byte r = Wire.endTransmission();
Serial.printf("cardkb_availability: %d\n",r);
    return (r == 0);
}

static void class_cardkb_available(mrb_vm *vm, mrb_value *v, int argc)
{
    if(cardkb_availability(*v->instance->data)){
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

void class_cardkb_init()
{
    mrb_class *class_cardkb;
    class_cardkb = mrbc_define_class(0, "CardKBClass", mrbc_class_object);
    mrbc_define_method(0, class_cardkb, "initialize", class_cardkb_initialize);
    mrbc_define_method(0, class_cardkb, "new", class_cardkb_new);
    mrbc_define_method(0, class_cardkb, "addr", class_cardkb_getaddr);
    mrbc_define_method(0, class_cardkb, "getch", class_cardkb_getch);
    mrbc_define_method(0, class_cardkb, "available?", class_cardkb_available);

    if(cardkb_availability(default_cardkb_addr)){
        auto cardkb = mrbc_instance_new(0, class_cardkb, sizeof(int));
        *cardkb.instance->data = default_cardkb_addr; // btnB
        mrbc_set_const(mrbc_str_to_symid("CardKB"),&cardkb);
    } else {
        mrbc_set_const(mrbc_str_to_symid("CardKB"),&failed_object);
    }


}
 

#endif // USE_CARDKB