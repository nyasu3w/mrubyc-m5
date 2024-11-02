#include <M5Unified.h>
#include "my_mrubydef.h"
#include "c_display_button.h"

static void
class_display_set_text_size(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        int sz = val_to_i(vm, v, GET_ARG(1),argc);
        M5.Display.setTextSize(sz);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void
class_display_print(mrb_vm *vm, mrb_value *v, int argc)
{
    int r=0;
    for(int i=1; i<=argc; i++){
        const char* str = val_to_s(vm, v, GET_ARG(i),argc);
        r+=M5.Display.print(str);
    }
    SET_INT_RETURN(r);
}

static void
class_display_puts(mrb_vm *vm, mrb_value v[], int argc)
{
    int r=0;
    if(argc==0) {
        r+=M5.Display.println();
    }
    for(int i=1; i<=argc; i++){
        const char* str = val_to_s(vm, v, GET_ARG(i),argc);
        r+=M5.Display.println(str);
    }
    SET_INT_RETURN(r);
}

static void
class_display_clear(mrb_vm *vm, mrb_value *v, int argc)
{
    int color=0;
    if(argc>0){
        color = val_to_i(vm, v, GET_ARG(1),argc);
    }

    M5.Display.clearDisplay(color);
    M5.Display.setCursor(0,0);
    SET_TRUE_RETURN();
}


static void
class_display_set_text_color(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>1){
        M5.Display.setTextColor(val_to_i(vm, v, GET_ARG(1),argc),val_to_i(vm, v, GET_ARG(2),argc));
        SET_TRUE_RETURN();
    } else if(argc>0) {
        M5.Display.setTextColor(val_to_i(vm, v, GET_ARG(1),argc));
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void
class_display_set_cursor(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>1){
        M5.Display.setCursor(val_to_i(vm, v, GET_ARG(1),argc),val_to_i(vm, v, GET_ARG(2),argc));
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void
class_display_getCursor(mrb_vm *vm, mrb_value *v, int argc)
{
    mrbc_value ret = mrbc_array_new(vm, 2);
    mrbc_value x = mrbc_fixnum_value(M5.Display.getCursorX());
    mrbc_value y = mrbc_fixnum_value(M5.Display.getCursorY());
    mrbc_array_set(&ret, 0, &x);
    mrbc_array_set(&ret, 1, &y);
    SET_RETURN(ret);
}


static void
class_display_color_value(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>2){
        uint8_t r = val_to_i(vm, v, GET_ARG(1),argc);
        uint8_t g = val_to_i(vm, v, GET_ARG(2),argc);
        uint8_t b = val_to_i(vm, v, GET_ARG(3),argc);
        uint16_t color = ((r>>3)<<11) | ((g>>2)<<5) | (b>>3);
        SET_INT_RETURN(color);
    } else {
        SET_FALSE_RETURN();
    }
}

static void
class_display_dimension(mrb_vm *vm, mrb_value *v, int argc) {
    mrbc_value ret = mrbc_array_new(vm, 2);
    mrbc_value w = mrbc_fixnum_value(M5.Display.width());
    mrbc_array_push(&ret, &w);
    mrbc_value h = mrbc_fixnum_value(M5.Display.height());
    mrbc_array_push(&ret, &h);
    SET_RETURN(ret);
}


#ifdef USE_DISPLAY_GRAPHICS
static void
class_display_fill_rect(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>4){
        int x=val_to_i(vm, v, GET_ARG(1),argc);
        int y=val_to_i(vm, v, GET_ARG(2),argc);
        int w=val_to_i(vm, v, GET_ARG(3),argc);
        int h=val_to_i(vm, v, GET_ARG(4),argc);
        int color=val_to_i(vm, v, GET_ARG(5),argc);
        M5.Display.fillRect(x,y,w,h,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void
class_display_draw_rect(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>4){
        int x=val_to_i(vm, v, GET_ARG(1),argc);
        int y=val_to_i(vm, v, GET_ARG(2),argc);
        int w=val_to_i(vm, v, GET_ARG(3),argc);
        int h=val_to_i(vm, v, GET_ARG(4),argc);
        int color=val_to_i(vm, v, GET_ARG(5),argc);
        M5.Display.drawRect(x,y,w,h,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void
class_display_draw_line(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>4){
        int x1=val_to_i(vm, v, GET_ARG(1),argc);
        int y1=val_to_i(vm, v, GET_ARG(2),argc);
        int x2=val_to_i(vm, v, GET_ARG(3),argc);
        int y2=val_to_i(vm, v, GET_ARG(4),argc);
        int color=val_to_i(vm, v, GET_ARG(5),argc);
        M5.Display.drawLine(x1,y1,x2,y2,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}


static void
class_display_flll_circle(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>3){
        int x=val_to_i(vm, v, GET_ARG(1),argc);
        int y=val_to_i(vm, v, GET_ARG(2),argc);
        int r=val_to_i(vm, v, GET_ARG(3),argc);
        int color=val_to_i(vm, v, GET_ARG(4),argc);
        M5.Display.fillCircle(x,y,r,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void
class_display_draw_circle(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>3){
        int x=val_to_i(vm, v, GET_ARG(1),argc);
        int y=val_to_i(vm, v, GET_ARG(2),argc);
        int r=val_to_i(vm, v, GET_ARG(3),argc);
        int color=val_to_i(vm, v, GET_ARG(4),argc);
        M5.Display.drawCircle(x,y,r,color);
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}
#endif // USE_DISPLAY_GRAPHICS

static void
class_btn_is_pressed(mrb_vm *vm, mrb_value *v, int argc)
{
    int no = *v->instance->data;
    m5::Button_Class btns[]={
        M5.BtnA,M5.BtnB,M5.BtnC, 
#ifdef USE_FULL_BUTTONS
        M5.BtnEXT, M5.BtnPWR 
#endif
    };
    if(0<=no && no<sizeof(btns)){
        if(btns[no].isPressed()) {
            SET_TRUE_RETURN();
        } else {
            SET_FALSE_RETURN();
        }
    }
}

static void
class_btn_was_pressed(mrb_vm *vm, mrb_value *v, int argc)
{
   int no = *v->instance->data;
    m5::Button_Class btns[]={M5.BtnA,M5.BtnB,M5.BtnC};
    if(0<=no && no<sizeof(btns)){
        if(btns[no].wasPressed()) {
            SET_TRUE_RETURN();
        } else {
            SET_FALSE_RETURN();
        }
    }
}


static void
class_btn_no(mrb_vm *vm, mrb_value *v, int argc)
{
    SET_INT_RETURN(*v->instance->data);
}


void class_display_button_init()
{
    mrb_class *class_display;
    class_display = mrbc_define_class(0, "Display", mrbc_class_object);
    mrbc_define_method(0, class_display, "print", class_display_print);
    mrbc_define_method(0, class_display, "println", class_display_puts);
    mrbc_define_method(0, class_display, "puts", class_display_puts);
    mrbc_define_method(0, class_display, "set_text_size", class_display_set_text_size);
    mrbc_define_method(0, class_display, "set_text_color", class_display_set_text_color);
    mrbc_define_method(0, class_display, "clear", class_display_clear);
    mrbc_define_method(0, class_display, "set_cursor", class_display_set_cursor);
    mrbc_define_method(0, class_display, "get_cursor", class_display_getCursor);
    mrbc_define_method(0, class_display, "color565", class_display_color_value);
    mrbc_define_method(0, class_display, "dimension", class_display_dimension);

#ifdef USE_DISPLAY_GRAPHICS
    mrbc_define_method(0, class_display, "fill_rect", class_display_fill_rect);
    mrbc_define_method(0, class_display, "draw_rect", class_display_draw_rect);
    mrbc_define_method(0, class_display, "fill_circle", class_display_flll_circle);
    mrbc_define_method(0, class_display, "draw_circle", class_display_draw_circle);
    mrbc_define_method(0, class_display, "draw_line", class_display_draw_line);
#endif // USE_DISPLAY_GRAPHICS

    mrb_class *class_btn,*class_btna,*class_btnb,*class_btnc;
    class_btn = mrbc_define_class(0, "BtnClass", mrbc_class_object);
    mrbc_define_method(0, class_btn, "is_pressed?", class_btn_is_pressed);
    mrbc_define_method(0, class_btn, "was_pressed?", class_btn_was_pressed);
    mrbc_define_method(0, class_btn, "number", class_btn_no);

    mrbc_value btn = mrbc_instance_new(0, class_btn, sizeof(int));
    *btn.instance->data = 0; // btnA
    mrbc_set_const(mrbc_str_to_symid("BtnA"),&btn);

    btn = mrbc_instance_new(0, class_btn, sizeof(int));
    *btn.instance->data = 1; // btnB
    mrbc_set_const(mrbc_str_to_symid("BtnB"),&btn);

    btn = mrbc_instance_new(0, class_btn, sizeof(int));
    *btn.instance->data = 2; // btnC
    mrbc_set_const(mrbc_str_to_symid("BtnC"),&btn);

#ifdef USE_FULL_BUTTONS
    btn = mrbc_instance_new(0, class_btn, sizeof(int));
    *btn.instance->data = 3; // btnEXT
    mrbc_set_const(mrbc_str_to_symid("BtnEXT"),&btn);

    btn = mrbc_instance_new(0, class_btn, sizeof(int));
    *btn.instance->data = 4; // btnPWR
    mrbc_set_const(mrbc_str_to_symid("BtnPWR"),&btn);
#endif // USE_ FULL_BUTTONS

}