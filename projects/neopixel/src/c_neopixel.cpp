#include <M5Unified.h>

#include <Adafruit_NeoPixel.h>
#include "c_neopixel.h"

#include "my_mrubydef.h"


static void
class_neopixel_new(mrb_vm *vm, mrb_value *v, int argc)
{
    int pin = (argc>1)? val_to_i(vm, v, GET_ARG(2),argc):M5.getPin(m5::port_a_pin2); 

    if(argc>0){
        int num=val_to_i(vm, v, GET_ARG(1),argc);
        Adafruit_NeoPixel *strip = new Adafruit_NeoPixel(num, pin, NEO_GRB + NEO_KHZ800);
        mrbc_value obj = mrbc_instance_new(vm, v[0].cls, sizeof(Adafruit_NeoPixel*));
        *(Adafruit_NeoPixel**)obj.instance->data = strip;
        strip->setBrightness(20);
        strip->begin();
        mrbc_instance_call_initialize( vm, v, argc );
        SET_RETURN(obj);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError),"too few arguments");
        SET_FALSE_RETURN();
    }
}


static void
class_neopixel_set_pixel_color(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>3){
        Adafruit_NeoPixel *strip = *(Adafruit_NeoPixel**)v->instance->data;
        int n = val_to_i(vm, v, GET_ARG(1),argc);
        int r = val_to_i(vm, v, GET_ARG(2),argc);
        int g = val_to_i(vm, v, GET_ARG(3),argc);
        int b = val_to_i(vm, v, GET_ARG(4),argc);
        if(n<0 || n>=strip->numPixels()){
            mrbc_raisef(vm, MRBC_CLASS(ArgumentError),"out of range (%d)",n);
            SET_FALSE_RETURN();
            return;
        }
        strip->setPixelColor(n, r, g, b);
    } else {
        SET_FALSE_RETURN();
    }
}

static void
class_neopixel_show(mrb_vm *vm, mrb_value *v, int argc)
{
    Adafruit_NeoPixel *strip = *(Adafruit_NeoPixel**)v->instance->data;
    strip->show();
    SET_TRUE_RETURN();
}

static void
class_neopixel_fill(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>4){
        Adafruit_NeoPixel *strip = *(Adafruit_NeoPixel**)v->instance->data;
        int r = val_to_i(vm, v, GET_ARG(1),argc);
        int g = val_to_i(vm, v, GET_ARG(2),argc);
        int b = val_to_i(vm, v, GET_ARG(3),argc);
        int st = val_to_i(vm, v, GET_ARG(4),argc);
        int co = val_to_i(vm, v, GET_ARG(5),argc);
        strip->fill(strip->Color(r,g,b),st,co);
    } else {
        SET_FALSE_RETURN();
    }
}
static void
class_neopixel_clear(mrb_vm *vm, mrb_value *v, int argc)
{
    Adafruit_NeoPixel *strip = *(Adafruit_NeoPixel**)v->instance->data;
    strip->clear();
    SET_TRUE_RETURN();
}

void class_neopixel_init()
{
    mrb_class *class_neopixel= mrbc_define_class(0, "NeoPixel", mrbc_class_object);
    mrbc_define_method(0, class_neopixel, "new", class_neopixel_new);
    mrbc_define_method(0, class_neopixel, "set_pixel_color", class_neopixel_set_pixel_color);
    mrbc_define_method(0, class_neopixel, "show", class_neopixel_show);
    mrbc_define_method(0, class_neopixel, "fill", class_neopixel_fill);
    mrbc_define_method(0, class_neopixel, "clear", class_neopixel_clear);
}

