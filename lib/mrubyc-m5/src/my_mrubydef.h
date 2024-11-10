/****
 *  my_mrubydef.h
 *  configuration of my mruby/c library
*****/


#ifndef SUPPRESS_USEDEFS
//#define FULL_BUTTONS    // to enable BtnEXT and BtnPWR
#define USE_DISPLAY_GRAPHICS  // Display to support graphics functions
#define USE_M5UNIFIED_IMU  // IMU to support M5Unified IMU functions
#define USE_SD_FUNCTION  // to support SD functions
#define USE_FILE_CLASS  // to support File functions
#define USE_TOUCH // to support Touch functions
#define USE_SPEAKER // to support Speaker functions
#endif // ! SUPPRESS_USEDEF

#define USE_TEMPORAL_RANDOM_FUNCTION  // it seems that mruby/c does not have randome number function


#include "mrubyc.h"

#include "c_m5.h"

inline const char* val_to_s(mrb_vm *vm,  mrb_value *v, mrbc_value &recv, int regofs) {
    const char* str;
    if( recv.tt == MRBC_TT_STRING ) {
        str = (const char*) recv.string->data;
    } else {
        mrbc_value val = mrbc_send(vm,v,regofs,&recv,"to_s",0);
        str = (const char*) val.string->data;
    }
    return str;
}
inline int val_to_i(mrb_vm *vm,  mrb_value *v, mrbc_value &recv, int regofs) {
    int i;
    if( recv.tt == MRBC_TT_FIXNUM ) {
        i = recv.i;
    } else {
        mrbc_value val = mrbc_send(vm,v,regofs,&recv,"to_i",0);
        i = val.i;
    }
    return i;
}

inline float val_to_f(mrb_vm *vm,  mrb_value *v, mrbc_value &recv, int regofs) {
    float f;
    if( recv.tt == MRBC_TT_FLOAT ) {
        f = recv.d;
    } else {
        mrbc_value val = mrbc_send(vm,v,regofs,&recv,"to_f",0);
        f = val.d;
    }
    return f;
}
