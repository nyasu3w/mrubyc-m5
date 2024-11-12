#include <M5Unified.h>
#include "my_mrubydef.h"

#ifdef USE_CANVAS
#include "c_canvas.h"
#include "drawing.h"

static void c_canvas_new(mrb_vm *vm, mrb_value *v, int argc) {
    v[0] = mrbc_instance_new(vm, v[0].cls, sizeof(M5Canvas));
    mrbc_instance_call_initialize(vm, v, argc);
}

static void c_canvas_initialize(mrb_vm *vm, mrb_value *v, int argc) {
    M5Canvas *canvas = new M5Canvas(&M5.Display);
    *(M5Canvas **)v->instance->data = canvas;
 
   if (argc > 2) {
        int depth = val_to_i(vm, v, GET_ARG(3), argc);
        canvas->setColorDepth(depth);
   } else if(argc==2) {
        int width = val_to_i(vm, v, GET_ARG(1), argc);
        int height = val_to_i(vm, v, GET_ARG(2), argc);
        canvas->createSprite(width,height);
    }else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "w/h");
    }
 
    canvas->setFont(&fonts::lgfxJapanGothic_12);
}


static void c_canvas_create_sprite(mrb_vm *vm, mrb_value *v, int argc) {
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    if (argc == 2) {
        int width = val_to_i(vm, v, GET_ARG(1), argc);
        int height = val_to_i(vm, v, GET_ARG(2), argc);
        canvas->createSprite(width,height);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "width must be specified");
        SET_FALSE_RETURN();
    }
}

static void c_canvas_scroll(mrb_vm *vm, mrb_value *v, int argc) {
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    if (argc == 2) {
        int dx = val_to_i(vm, v, GET_ARG(1), argc);
        int dy = val_to_i(vm, v, GET_ARG(2), argc);
        canvas->scroll(dx, dy);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "dx and dy");
        SET_FALSE_RETURN();
    }
}

static void c_canvas_push_sprite_to_display(mrb_vm *vm, mrb_value *v, int argc) {
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    if(argc == 2) {
        int x = val_to_i(vm, v, GET_ARG(1), argc);
        int y = val_to_i(vm, v, GET_ARG(2), argc);
        canvas->pushSprite(x, y);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "x-y");
        SET_FALSE_RETURN();
    }
}

static void c_canvas_delete_sprite(mrb_vm *vm, mrb_value *v, int argc) {
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    canvas->deleteSprite();
}



static void c_canvas_set_text_color(mrb_vm *vm, mrb_value *v, int argc) {
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_set_text_color(canvas, vm, v, argc);
}

static void c_canvas_set_text_size(mrb_vm *vm, mrb_value *v, int argc) {
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_set_text_size(canvas, vm, v, argc);
}

static void c_canvas_set_cursor(mrb_vm *vm, mrb_value *v, int argc) {
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_set_cursor(canvas, vm, v, argc);
}

static void c_canvas_get_cursor(mrb_vm *vm, mrb_value *v, int argc) {
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_get_cursor(canvas, vm, v, argc);
}


static void c_canvas_print(mrb_vm *vm, mrb_value *v, int argc) {
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_print(canvas, vm, v, argc);
}

static void c_canvas_puts(mrb_vm *vm, mrb_value *v, int argc) {
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_puts(canvas, vm, v, argc);
}

static void c_canvas_clear(mrb_vm *vm, mrb_value *v, int argc) {
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_clear(canvas, vm, v, argc);
}


static void class_display_fill_rect(mrb_vm *vm, mrb_value *v, int argc)
{
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_fill_rect(canvas,vm,v,argc);
}

static void class_display_draw_rect(mrb_vm *vm, mrb_value *v, int argc)
{
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_draw_rect(canvas,vm,v,argc);
}

static void class_display_flll_circle(mrb_vm *vm, mrb_value *v, int argc)
{
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_flll_circle(canvas,vm,v,argc);
}

static void class_display_draw_circle(mrb_vm *vm, mrb_value *v, int argc)
{
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_draw_circle(canvas,vm,v,argc);
}

static void class_display_draw_line(mrb_vm *vm, mrb_value *v, int argc)
{
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_draw_line(canvas,vm,v,argc);
}

static void class_display_draw_bmp(mrb_vm *vm, mrb_value *v, int argc)
{
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_draw_bmp(canvas,vm,v,argc);
}

static void class_display_draw_jpg(mrb_vm *vm, mrb_value *v, int argc)
{
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_draw_jpg(canvas,vm,v,argc);
}

static void class_display_draw_png(mrb_vm *vm, mrb_value *v, int argc)
{
    M5Canvas *canvas = *(M5Canvas **)v->instance->data;
    draw_draw_png(canvas,vm,v,argc);
}

void class_canvas_init() {
    // define class
    mrbc_class *canvas_class= mrbc_define_class(0,"Canvas", mrbc_class_object);
    mrbc_define_method(0, canvas_class, "new", c_canvas_new);
    mrbc_define_method(0, canvas_class, "initialize", c_canvas_initialize);
    mrbc_define_method(0, canvas_class, "scroll", c_canvas_scroll);
    mrbc_define_method(0, canvas_class, "push_sprite", c_canvas_push_sprite_to_display);
    mrbc_define_method(0, canvas_class, "delete_sprite", c_canvas_delete_sprite);

    mrbc_define_method(0, canvas_class, "set_text_color", c_canvas_set_text_color);
    mrbc_define_method(0, canvas_class, "set_text_size", c_canvas_set_text_size);
    mrbc_define_method(0, canvas_class, "set_cursor", c_canvas_set_cursor);
    mrbc_define_method(0, canvas_class, "get_cursor", c_canvas_get_cursor);
    mrbc_define_method(0, canvas_class, "print", c_canvas_print);
    mrbc_define_method(0, canvas_class, "puts", c_canvas_puts);
    mrbc_define_method(0, canvas_class, "clear", c_canvas_clear);

    mrbc_define_method(0, canvas_class, "fill_rect", class_display_fill_rect);
    mrbc_define_method(0, canvas_class, "draw_rect", class_display_draw_rect);
    mrbc_define_method(0, canvas_class, "fill_circle", class_display_flll_circle);
    mrbc_define_method(0, canvas_class, "draw_circle", class_display_draw_circle);
    mrbc_define_method(0, canvas_class, "draw_line", class_display_draw_line);
    mrbc_define_method(0, canvas_class, "draw_bmpfile", class_display_draw_bmp);
    mrbc_define_method(0, canvas_class, "draw_jpgfile", class_display_draw_jpg);
    mrbc_define_method(0, canvas_class, "draw_pngfile", class_display_draw_png);

}

#endif // USE_CANVAS