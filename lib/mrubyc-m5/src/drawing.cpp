//
// Common drawing functions for M5GFX, M5Canvas
//

#include <M5Unified.h>
#include "my_mrubydef.h"

#include "drawing.h"
#include "c_font.h"
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

static void draw_draw_pic_stream(LovyanGFX *dst, draw_pic_type t, Stream* instream, int x, int y){
    switch(t){
        case bmp:
            dst->drawBmp(instream,x,y);
            break;
        case jpg:
            dst->drawJpg(instream,x,y);
            break;
        case png:
            dst->drawPng(instream,x,y);
            break;
    }
}


static void draw_draw_pic_file(LovyanGFX *dst, draw_pic_type t, mrb_vm *vm, mrb_value *v, int argc)
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

    draw_draw_pic_stream(dst,t,f,x,y);

    SET_TRUE_RETURN();
}


void draw_draw_bmp(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    draw_draw_pic_file(dst,bmp,vm,v,argc);
}

void draw_draw_jpg(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    draw_draw_pic_file(dst, jpg,vm,v,argc);
}

void draw_draw_png(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    draw_draw_pic_file(dst, png,vm,v,argc);
}

#include <Stream.h>
// ByteStream is a class that inherits from Stream 
class ByteStream : public Stream {  // implemented almost by copilot
private:
    const uint8_t *str;
public:
    uint16_t pos,length;

    ByteStream(const uint8_t* s, uint16_t len) : str(s), pos(0), length(len) {}
    int available() { return length - pos; }
    int read() { return ((pos < length)? str[pos++] : -1); }
    int peek() { return ((pos < length)? str[pos] : -1); }
    size_t write(uint8_t c) { return 0;}  // do nothing
};

void draw_draw_pic_str(LovyanGFX *dst, draw_pic_type t, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc<3){
        mrbc_raise(vm, MRBC_CLASS(ArgumentError),"too few arguments");
        return;
    }
    if(GET_ARG(1).tt != MRBC_TT_STRING){
        mrbc_raise(vm, MRBC_CLASS(ArgumentError),"not a string");
        SET_FALSE_RETURN();
        return;
    }

    ByteStream bs(GET_ARG(1).string->data, GET_ARG(1).string->size);
    int x = val_to_i(vm, v, GET_ARG(2),argc);
    int y = val_to_i(vm, v, GET_ARG(3),argc);

    draw_draw_pic_stream(dst,t,&bs,x,y);
    
    SET_TRUE_RETURN();
}

void draw_draw_bmpstr(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    draw_draw_pic_str(dst,bmp,vm,v,argc);
}

void draw_draw_jpgstr(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    draw_draw_pic_str(dst, jpg,vm,v,argc);
}

void draw_draw_pngstr(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    draw_draw_pic_str(dst, png,vm,v,argc);
}

void draw_set_font(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        unsigned int  fontnumber = val_to_i(vm, v, GET_ARG(1),argc);
        if(fontnumber < max_font_number){
            dst->setFont(supported_fonts[fontnumber]);
            SET_TRUE_RETURN();
            return;
        }
    }
    SET_FALSE_RETURN();
}

void draw_set_rotation(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        int rotation = val_to_i(vm, v, GET_ARG(1),argc);
        dst->setRotation(rotation);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

void draw_get_dimension(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    mrbc_value ret = mrbc_array_new(vm, 2);
    mrbc_value w = mrbc_fixnum_value(dst->width());
    mrbc_value h = mrbc_fixnum_value(dst->height());
    mrbc_array_set(&ret, 0, &w);
    mrbc_array_set(&ret, 1, &h);
    SET_RETURN(ret);
}

void draw_scroll(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if (argc == 2) {
        int dx = val_to_i(vm, v, GET_ARG(1), argc);
        int dy = val_to_i(vm, v, GET_ARG(2), argc);
        dst->scroll(dx, dy);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "dx and dy");
        SET_FALSE_RETURN();
    }
}
