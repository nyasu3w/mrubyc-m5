#include <M5Unified.h>

#include <Adafruit_NeoPixel.h>
#include "c_neopixel.h"

#include "my_mrubydef.h"


static void
class_neopixel_new(mrb_vm *vm, mrb_value *v, int argc)
{
    int pin = (argc>1)? MRBC_ARG_I(2):M5.getPin(m5::port_a_pin2); 

    if(argc>0){
        int num=MRBC_ARG_I(1);
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
        Adafruit_NeoPixel *strip = get_checked_data(Adafruit_NeoPixel, vm, v);
        int n = MRBC_ARG_I(1);
        int r = MRBC_ARG_I(2);
        int g = MRBC_ARG_I(3);
        int b = MRBC_ARG_I(4);
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
    Adafruit_NeoPixel *strip = get_checked_data(Adafruit_NeoPixel, vm, v);
    strip->show();
    SET_TRUE_RETURN();
}

static void
class_neopixel_fill(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>4){
        Adafruit_NeoPixel *strip = get_checked_data(Adafruit_NeoPixel, vm, v);
        int r = MRBC_ARG_I(1);
        int g = MRBC_ARG_I(2);
        int b = MRBC_ARG_I(3);
        int st = MRBC_ARG_I(4);
        int co = MRBC_ARG_I(5);
        strip->fill(strip->Color(r,g,b),st,co);
    } else {
        SET_FALSE_RETURN();
    }
}
static void
class_neopixel_clear(mrb_vm *vm, mrb_value *v, int argc)
{
    Adafruit_NeoPixel *strip = get_checked_data(Adafruit_NeoPixel, vm, v);
    strip->clear();
    SET_TRUE_RETURN();
}

static void
class_neopixel_map1(mrb_vm *vm, mrb_value *v, int argc)
{
    Adafruit_NeoPixel *strip = get_checked_data(Adafruit_NeoPixel, vm, v);
    int sz = strip->numPixels();

    if(argc<1) {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "too few arguments");
        SET_FALSE_RETURN();
        return;
    }

    if(GET_ARG(1).tt != MRBC_TT_ARRAY) {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "argument1 is not Array");
        SET_FALSE_RETURN();
        return;
    }

    unsigned int alen = GET_ARG(1).array->n_stored;

    int offset = (argc>1)? MRBC_ARG_I(2) : 0;
    if(offset<0) offset = 0;
    unsigned int p=(int)offset;

    while(p < sz && p-offset < alen){
        mrbc_value ary = mrbc_array_get(&GET_ARG(1),p);
        if(ary.tt == MRBC_TT_ARRAY && ary.array->n_stored>=3){
            mrbc_value va;
            va = mrbc_array_get(&ary,0);
            int r =MRBC_TO_I(va);
            va = mrbc_array_get(&ary,1);
            int g = MRBC_TO_I(va);
            va = mrbc_array_get(&ary,2);
            int b =MRBC_TO_I(va);
            strip->setPixelColor(p, strip->Color(r,g,b));
        } else {
            mrbc_raisef(vm, MRBC_CLASS(ArgumentError), "argument1 is not in the format [%d]",p);
            SET_FALSE_RETURN();
            return;
        }
        p++;
    }
}

static void
class_neopixel_map3(mrb_vm *vm, mrb_value *v, int argc)
{
    Adafruit_NeoPixel *strip = get_checked_data(Adafruit_NeoPixel, vm, v);
    int sz = strip->numPixels();

    if(argc<3) {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "too few arguments");
        SET_FALSE_RETURN();
        return;
    }
    unsigned int minlen = sz;
    for(int i=1;i<3;i++) {
        if(GET_ARG(i).tt != MRBC_TT_ARRAY) {
            mrbc_raisef(vm, MRBC_CLASS(ArgumentError), "argument%d is not an Array",i);
            SET_FALSE_RETURN();
            return;
        }
        unsigned int arysize = GET_ARG(i).array->n_stored;
        if(arysize<minlen) minlen = arysize;
    }

    mrbc_value ra = GET_ARG(1);
    mrbc_value ga = GET_ARG(2);
    mrbc_value ba = GET_ARG(3);

    int offset = (argc>3)? MRBC_ARG_I(4) : 0;
    if(offset<0) offset = 0;
    unsigned int p=(int)offset;

    while(p < sz && p - offset < minlen){
        mrbc_value va;
        va = mrbc_array_get(&ra,p);
        int r =MRBC_TO_I(va);
        va = mrbc_array_get(&ga,p);
        int g =MRBC_TO_I(va);
        va = mrbc_array_get(&ba,p);
        int b = MRBC_TO_I(va);
        strip->setPixelColor(p, strip->Color(r,g,b));
        p++;
    }
}

static void
class_neopixel_destroy(mrb_vm *vm, mrb_value *v, int argc)
{
    Adafruit_NeoPixel *strip = get_checked_data(Adafruit_NeoPixel, vm, v);
    strip->~Adafruit_NeoPixel();
    delete strip;
    put_null_data(v);
}

void class_neopixel_init()
{
    mrb_class *class_neopixel= mrbc_define_class(0, "NeoPixel", mrbc_class_object);
    mrbc_define_method(0, class_neopixel, "new", class_neopixel_new);
    mrbc_define_method(0, class_neopixel, "set_pixel_color", class_neopixel_set_pixel_color);
    mrbc_define_method(0, class_neopixel, "show", class_neopixel_show);
    mrbc_define_method(0, class_neopixel, "fill", class_neopixel_fill);
    mrbc_define_method(0, class_neopixel, "clear", class_neopixel_clear);
    mrbc_define_method(0, class_neopixel, "map1", class_neopixel_map1);
    mrbc_define_method(0, class_neopixel, "map3", class_neopixel_map3);
    mrbc_define_method(0, class_neopixel, "destroy", class_neopixel_destroy);
}

