

// The followins are estimated to be included before including this file.
// #include <mrubyc.h>
// #include <M5Unified.h>

typedef enum{bmp,jpg,png} draw_pic_type;

void draw_set_text_size(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_print(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_puts(LovyanGFX *dst, mrb_vm *vm, mrb_value v[], int argc);
void draw_clear(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_set_text_color(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_set_cursor(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_get_cursor(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_fill_rect(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_draw_rect(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_draw_line(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_flll_circle(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_draw_circle(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_draw_bmp(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_draw_jpg(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_draw_png(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_draw_bmpstr(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_draw_jpgstr(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
void draw_draw_pngstr(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc);
