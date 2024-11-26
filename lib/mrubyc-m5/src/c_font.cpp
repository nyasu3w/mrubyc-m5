
#include <M5Unified.h>
#include "my_mrubydef.h"
#include "c_font.h"

#ifdef USE_FONT

#include "drawing.h"

/* These enum and array supported_fonts must be paired */
/* To make another font supported, add the entry on the both, and implement it in class_font_by_name */
/* Too many fonts will exhause flash area */
typedef enum {
    Default=0,
    DejaVu12,
    efontJA_12, 
    // efontCN_12, efontKR_12,efontTW_12,
    last_sentinel  // must be the last
} fontnumber_t;

const lgfx::v1::IFont* supported_fonts[] = {
    nullptr,  // Default
    &fonts::DejaVu12,
    &fonts::efontJA_12,
    //&fonts::efontCN_12, &fonts::efontKR_12, &fonts::efontTW_12, 
    nullptr  // must be the last
};

uint16_t max_font_number = fontnumber_t::last_sentinel;

static void
class_font_by_name(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        const char* fontname = val_to_s(vm, v, GET_ARG(1),argc);
        fontnumber_t font = Default;
        // to add alias, add the if statement here
        if(strncmp(fontname,"DejaVu12",9)==0) font = fontnumber_t::DejaVu12;
        else if(strncmp(fontname,"efontJA_12",11)==0) font = fontnumber_t::efontJA_12;
/* commented out because the glyphs of 直 looks the same as efontJA_12
        else if(strncmp(fontname,"efontCN_12",11)==0) font = fontnumber_t::efontCN_12;
        else if(strncmp(fontname,"efontKR_12",11)==0) font = fontnumber_t::efontKR_12;
        else if(strncmp(fontname,"efontTW_12",11)==0) font = fontnumber_t::efontTW_12;
*/
        SET_INT_RETURN(font);
    } else {
        SET_FALSE_RETURN();
    }
}

static void class_display_set_font(mrb_vm *vm, mrb_value *v, int argc)
{
    draw_set_font(&M5.Display,vm,v,argc);
}

static void class_canvas_set_font(mrb_vm *vm, mrb_value *v, int argc)
{
    M5Canvas *canvas =get_checked_data(M5Canvas,vm, v);
    draw_set_font(canvas,vm,v,argc);
}

void class_font_init()  // order dependency. Font must be defined after Display and Canvas
{
    mrb_class *class_font = mrbc_define_class(0, "Font", mrbc_class_object);
    mrbc_define_method(0, class_font, "by_name", class_font_by_name);

    mrbc_class *class_display = mrbc_get_class_by_name("Display");
    mrbc_class *class_canvas = mrbc_get_class_by_name("Canvas");
    mrbc_define_method(0, class_display, "set_font", class_display_set_font);
    mrbc_define_method(0, class_canvas, "set_font", class_canvas_set_font);
}

#endif // USE_FONT