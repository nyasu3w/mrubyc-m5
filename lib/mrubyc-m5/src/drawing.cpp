//
// Common drawing functions for M5GFX, M5Canvas
//

#include "my_mrubydef.h"

#ifdef USE_SD_FUNCTION
#include <SD.h>
#endif

#ifdef USE_SPIFFS_FUNCTION
#include <SPIFFS.h>
#endif

#include <M5Unified.h>

#include "drawing.h"
#include "c_font.h"
#include "c_file.h"


void draw_set_text_size(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        int sz = MRBC_ARG_I(1);
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
        const char* str = MRBC_ARG_S(i);
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
        const char* str = MRBC_ARG_S(i);
        r+=dst->println(str);
    }
    SET_INT_RETURN(r);
}


void draw_clear(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    int color=0;
    if(argc>0){
        color = MRBC_ARG_I(1);
    }

    dst->clearDisplay(color);
    dst->setCursor(0,0);
    SET_TRUE_RETURN();
}



void draw_set_text_color(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>1){
        dst->setTextColor(MRBC_ARG_I(1),MRBC_ARG_I(2));
        SET_TRUE_RETURN();
    } else if(argc>0) {
        dst->setTextColor(MRBC_ARG_I(1));
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}


void draw_set_cursor(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>1){
        dst->setCursor(MRBC_ARG_I(1),MRBC_ARG_I(2));
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}


void draw_get_cursor(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    mrbc_value ret = mrbc_array_new(vm, 2);
    mrbc_value x = mrbc_fixnum_value(dst->getCursorX());
    mrbc_value y = mrbc_fixnum_value(dst->getCursorY());
    mrbc_array_set(&ret, 0, &x);
    mrbc_array_set(&ret, 1, &y);
    SET_RETURN(ret);
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

//
// for USE_DISPLAY_GRAPHICS
//

#ifdef USE_DISPLAY_GRAPHICS
void draw_fill_rect(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>4){
        int x=MRBC_ARG_I(1);
        int y=MRBC_ARG_I(2);
        int w=MRBC_ARG_I(3);
        int h=MRBC_ARG_I(4);
        int color=MRBC_ARG_I(5);
        dst->fillRect(x,y,w,h,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}


void draw_draw_rect(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>4){
        int x=MRBC_ARG_I(1);
        int y=MRBC_ARG_I(2);
        int w=MRBC_ARG_I(3);
        int h=MRBC_ARG_I(4);
        int color=MRBC_ARG_I(5);
        dst->drawRect(x,y,w,h,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}


void draw_draw_line(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>4){
        int x1=MRBC_ARG_I(1);
        int y1=MRBC_ARG_I(2);
        int x2=MRBC_ARG_I(3);
        int y2=MRBC_ARG_I(4);
        int color=MRBC_ARG_I(5);
        dst->drawLine(x1,y1,x2,y2,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}



void draw_flll_circle(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>3){
        int x=MRBC_ARG_I(1);
        int y=MRBC_ARG_I(2);
        int r=MRBC_ARG_I(3);
        int color=MRBC_ARG_I(4);
        dst->fillCircle(x,y,r,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}


void draw_draw_circle(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>3){
        int x=MRBC_ARG_I(1);
        int y=MRBC_ARG_I(2);
        int r=MRBC_ARG_I(3);
        int color=MRBC_ARG_I(4);
        dst->drawCircle(x,y,r,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

#if defined(USE_SD_FUNCTION) || defined(USE_SPIFFS_FUNCTION)

static uint8_t draw_draw_pic_from_path(LovyanGFX *dst, draw_pic_type t, mrb_value* storage, const char* path, int32_t x, int32_t y,float scale_x=1.0, float scale_y=0.0 ){
    if(storage == &failed_object) {
        return 1;
    }
    enum {STO_NONE=0, STO_SPIFFS, STO_SDCARD } sto_class;
    if(mrbc_obj_is_kind_of(storage, mrbc_get_class_by_name("SPIFFS"))) {
        sto_class = STO_SPIFFS;
    } else if(mrbc_obj_is_kind_of(storage, mrbc_get_class_by_name("SD"))) {
        sto_class=STO_SDCARD;
    } else {  // path error
        return 1;
    }

    switch(t){
        case bmp:
#ifdef USE_SPIFFS_FUNCTION
            if(sto_class==STO_SPIFFS)
                dst->drawBmpFile(SPIFFS,path,x,y,0,0,0,0,scale_x,scale_y);
#endif
#ifdef USE_SD_FUNCTION
            if(sto_class==STO_SDCARD)
                dst->drawBmpFile(SD,path,x,y,0,0,0,0,scale_x,scale_y);
#endif
                break;
        case jpg:
#ifdef USE_SPIFFS_FUNCTION
            if(sto_class==STO_SPIFFS)
                dst->drawJpgFile(SPIFFS,path,x,y,0,0,0,0,scale_x,scale_y);
#endif
#ifdef USE_SD_FUNCTION
            if(sto_class==STO_SDCARD)
                dst->drawJpgFile(SD,path,x,y,0,0,0,0,scale_x,scale_y);
#endif
            break;
        case png:
#ifdef USE_SPIFFS_FUNCTION
            if(sto_class==STO_SPIFFS)
                dst->drawPngFile(SPIFFS,path,x,y,0,0,0,0,scale_x,scale_y);
#endif
#ifdef USE_SD_FUNCTION
            if(sto_class==STO_SDCARD)
                dst->drawPngFile(SD,path,x,y,0,0,0,0,scale_x,scale_y);
#endif
            break;
    }
    return 0;
}

static void draw_draw_pic_file(LovyanGFX *dst, draw_pic_type t, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc<4){
        mrbc_raise(vm, MRBC_CLASS(ArgumentError),"too few arguments");
        return;
    }

    const char* path = MRBC_ARG_S(2);
    int x = MRBC_ARG_I(3);
    int y = MRBC_ARG_I(4);

    float scale_x=1.0, scale_y=0.0;
    if(argc>4){
        scale_x = MRBC_ARG_F(5);
    } 
    if(argc>5){
        scale_y = MRBC_ARG_F(6);
    } 

    if(draw_draw_pic_from_path(dst,t,MRBC_ARG(1), path,x,y,scale_x,scale_y)){
        // now only retcode:1 is implemented
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "wrong path or storage");
    }

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

#endif // USE_SD_FUNCTION || SPIFFS_FUNCTIOn

static void draw_draw_pic_mem(LovyanGFX *dst, draw_pic_type t, const uint8_t * mem, size_t memsize, int x, int y){
    switch(t){
        case bmp:
            dst->drawBmp(mem,memsize,x,y);
            break;
        case jpg:
            dst->drawJpg(mem,memsize,x,y);;
            break;
        case png:
            dst->drawPng(mem,memsize,x,y);;
            break;
    }
}

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
    const uint8_t *mem = GET_ARG(1).string->data;
    size_t memsize = GET_ARG(1).string->size;

    int x = MRBC_ARG_I(2);
    int y = MRBC_ARG_I(3);

    draw_draw_pic_mem(dst,t, mem, memsize,x,y);
    
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

void draw_set_rotation(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        int rotation = MRBC_ARG_I(1);
        dst->setRotation(rotation);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

void draw_scroll(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    if (argc == 2) {
        int dx = MRBC_ARG_I(1);
        int dy = MRBC_ARG_I(2);
        dst->scroll(dx, dy);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "dx and dy");
        SET_FALSE_RETURN();
    }
}

void draw_set_scroll_rect(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{
    int x=0, y=0;
    int w=dst->width();
    int h=dst->height();

    if(argc>0){
        x = MRBC_ARG_I(1);
    } 
    if(argc>1){
        y = MRBC_ARG_I(2);
    } 
    if(argc>2){
        w = MRBC_ARG_I(3);
    } 
    if(argc>3){
        h = MRBC_ARG_I(4);
    } 

    dst->setScrollRect(x,y,w,h);
}

void draw_set_text_scroll(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc)
{

    int f = true;
    if(argc>0) f = MRBC_ARG_I(1);
    dst->setTextScroll(f!=0);
}


#endif // USE_DISPLAY_GRAPHICS
