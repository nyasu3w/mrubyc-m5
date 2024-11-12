//
// Common drawing functions for M5GFX, M5Canvas
//

#include <M5Unified.h>
#include "my_mrubydef.h"

#include "drawing.h"
#include "c_file.h"


void draw_set_text_size(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        int sz = val_to_i(vm, v, GET_ARG(1),argc);
        dst->setTextSize(sz);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}


void draw_print(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    int r=0;
    for(int i=1; i<=argc; i++){
        const char* str = val_to_s(vm, v, GET_ARG(i),argc);
        r+=dst->print(str);
    }
    SET_INT_RETURN(r);
}


void draw_puts(LovyanGFX *dst, mrb_vm *vm, mrb_value v[], int argc)
{
    int r=0;
    if(argc==0) {
        r+=dst->println();
    }
    for(int i=1; i<=argc; i++){
        const char* str = val_to_s(vm, v, GET_ARG(i),argc);
        r+=dst->println(str);
    }
    SET_INT_RETURN(r);
}


void draw_clear(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    int color=0;
    if(argc>0){
        color = val_to_i(vm, v, GET_ARG(1),argc);
    }

    dst->clearDisplay(color);
    dst->setCursor(0,0);
    SET_TRUE_RETURN();
}



void draw_set_text_color(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>1){
        dst->setTextColor(val_to_i(vm, v, GET_ARG(1),argc),val_to_i(vm, v, GET_ARG(2),argc));
        SET_TRUE_RETURN();
    } else if(argc>0) {
        dst->setTextColor(val_to_i(vm, v, GET_ARG(1),argc));
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}


void draw_set_cursor(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>1){
        dst->setCursor(val_to_i(vm, v, GET_ARG(1),argc),val_to_i(vm, v, GET_ARG(2),argc));
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}


void draw_get_cursor(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    mrbc_value ret = mrbc_array_new(vm, 2);
    mrbc_value x = mrbc_fixnum_value(M5.Display.getCursorX());
    mrbc_value y = mrbc_fixnum_value(M5.Display.getCursorY());
    mrbc_array_set(&ret, 0, &x);
    mrbc_array_set(&ret, 1, &y);
    SET_RETURN(ret);
}


//
// for USE_DISPLAY_GRAPHICS
//


void draw_fill_rect(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>4){
        int x=val_to_i(vm, v, GET_ARG(1),argc);
        int y=val_to_i(vm, v, GET_ARG(2),argc);
        int w=val_to_i(vm, v, GET_ARG(3),argc);
        int h=val_to_i(vm, v, GET_ARG(4),argc);
        int color=val_to_i(vm, v, GET_ARG(5),argc);
        dst->fillRect(x,y,w,h,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}


void draw_draw_rect(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>4){
        int x=val_to_i(vm, v, GET_ARG(1),argc);
        int y=val_to_i(vm, v, GET_ARG(2),argc);
        int w=val_to_i(vm, v, GET_ARG(3),argc);
        int h=val_to_i(vm, v, GET_ARG(4),argc);
        int color=val_to_i(vm, v, GET_ARG(5),argc);
        dst->drawRect(x,y,w,h,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}


void draw_draw_line(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>4){
        int x1=val_to_i(vm, v, GET_ARG(1),argc);
        int y1=val_to_i(vm, v, GET_ARG(2),argc);
        int x2=val_to_i(vm, v, GET_ARG(3),argc);
        int y2=val_to_i(vm, v, GET_ARG(4),argc);
        int color=val_to_i(vm, v, GET_ARG(5),argc);
        dst->drawLine(x1,y1,x2,y2,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}



void draw_flll_circle(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>3){
        int x=val_to_i(vm, v, GET_ARG(1),argc);
        int y=val_to_i(vm, v, GET_ARG(2),argc);
        int r=val_to_i(vm, v, GET_ARG(3),argc);
        int color=val_to_i(vm, v, GET_ARG(4),argc);
        dst->fillCircle(x,y,r,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}


void draw_draw_circle(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>3){
        int x=val_to_i(vm, v, GET_ARG(1),argc);
        int y=val_to_i(vm, v, GET_ARG(2),argc);
        int r=val_to_i(vm, v, GET_ARG(3),argc);
        int color=val_to_i(vm, v, GET_ARG(4),argc);
        dst->drawCircle(x,y,r,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

void draw_draw_pic(LovyanGFX *dst, draw_pic_type t, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc<3){
        mrbc_raise(vm, MRBC_CLASS(ArgumentError),"too few arguments");
        return;
    }
    mrbc_value file = GET_ARG(1);
    int r = mrbc_obj_is_kind_of(&file, class_file);
    if(r==0){
        mrbc_raise(vm, MRBC_CLASS(ArgumentError),"not a file");
        SET_FALSE_RETURN();
        return;
    }
    File *f = *(File**) file.instance->data;
    int x = val_to_i(vm, v, GET_ARG(2),argc);
    int y = val_to_i(vm, v, GET_ARG(3),argc);
    switch(t){
        case bmp:
            dst->drawBmp(f,x,y);
            break;
        case jpg:
            dst->drawJpg(f,x,y);
            break;
        case png:
            dst->drawPng(f,x,y);
            break;
    }
    SET_TRUE_RETURN();
}


void draw_draw_bmp(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    draw_draw_pic(dst,bmp,vm,v,argc);
}


void draw_draw_jpg(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    draw_draw_pic(dst, jpg,vm,v,argc);
}


void draw_draw_png(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    draw_draw_pic(dst, png,vm,v,argc);
}
