#include "c_roller485.h"
#include "my_mrubydef.h"

#include <M5Unified.h>
#include <Wire.h>
#include <unit_rolleri2c.hpp>

UnitRollerI2C Roller;

static bool default_roller485_init() {

    if (Roller.begin(&Wire, 0x64, M5.getPin(m5::port_a_pin1),M5.getPin(m5::port_a_pin2), 400000)) {
        return true;
    }
    return false;
}

static void class_roller485_calibration(mrb_vm *vm, mrb_value *v, int argc) {

    printf("Start encoder calibration\n");
    Roller.setOutput(0);
    delay(100);
    Roller.startAngleCal();
    delay(100);
    printf("Calibrationing...\n");
    while (Roller.getCalBusyStatus()) {
        printf("Calibrationing...\n");
    }
    Roller.updateAngleCal();
    printf("Encoder calibration done\n");
}


// mode = 1:SPEED, 2:POSITION, 3:CURRENT, 4:ENCODER
static void class_roller485_setmode(mrb_vm *vm, mrb_value *v, int argc) {
    if (argc > 0) {
        int mode = val_to_i(vm, v, GET_ARG(1), argc);
        if(mode < 1 || mode > 4) {
            mrbc_raise(vm, MRBC_CLASS(ArgumentError), "mode");
            SET_FALSE_RETURN();
        }
        Roller.setMode((roller_mode_t) mode);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "setmode");
        SET_FALSE_RETURN();
    }
}
static void class_roller485_getmode(mrb_vm *vm, mrb_value *v, int argc) {
    int mode = Roller.getMotorMode();
    SET_INT_RETURN(mode);
}

static void class_roller485_setoutput(mrb_vm *vm, mrb_value *v, int argc) {
    if (argc > 0) {
        uint8_t output = val_to_i(vm, v, GET_ARG(1), argc);
        Roller.setOutput(output);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "output");
        SET_FALSE_RETURN();
    }
}

static void class_roller485_setspeed(mrb_vm *vm, mrb_value *v, int argc) {
    if (argc > 0) {
        int32_t speed = val_to_i(vm, v, GET_ARG(1), argc);
        Roller.setSpeed(speed);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "speed");
        SET_FALSE_RETURN();
    }
}

static void class_roller485_setspeedmaxcurrent(mrb_vm *vm, mrb_value *v, int argc) {
    if (argc > 0) {
        int32_t current = val_to_i(vm, v, GET_ARG(2), argc);
        Roller.setSpeedMaxCurrent(current);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "speed");
        SET_FALSE_RETURN();
    }
}

static void class_roller485_setcurrent(mrb_vm *vm, mrb_value *v, int argc) {
    if (argc > 0) {
        int32_t current = val_to_i(vm, v, GET_ARG(1), argc);
        Roller.setCurrent(current);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "current");
        SET_FALSE_RETURN();
    }
}

static void class_roller485_getcurrent(mrb_vm *vm, mrb_value *v, int argc) {
        int32_t current = Roller.getCurrent();
        SET_INT_RETURN(current);
}
static void class_roller485_getspeed(mrb_vm *vm, mrb_value *v, int argc) {
        int32_t speed = Roller.getSpeed();
        SET_INT_RETURN(speed);
}
static void class_roller485_current_readback(mrb_vm *vm, mrb_value *v, int argc) {
        int32_t current = Roller.getCurrentReadback();
        SET_INT_RETURN(current);
}
static void class_roller485_speed_readback(mrb_vm *vm, mrb_value *v, int argc) {
    int32_t speed = Roller.getSpeedReadback();
    SET_INT_RETURN(speed);
}
static void class_roller485_setpos(mrb_vm *vm, mrb_value *v, int argc) {
    if (argc > 0) {
        int32_t pos = val_to_i(vm, v, GET_ARG(1), argc);
        Roller.setPos(pos);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "pos");
        SET_FALSE_RETURN();
    }
}

static void class_roller485_getpos(mrb_vm *vm, mrb_value *v, int argc) {
    int32_t pos = Roller.getPos();
    SET_INT_RETURN(pos);
}
static void class_roller485_getpos_readback(mrb_vm *vm, mrb_value *v, int argc) {
    int32_t pos = Roller.getPosReadback();
    SET_INT_RETURN(pos);
}
static void class_roller485_getpospid(mrb_vm *vm, mrb_value *v, int argc) {
    uint32_t p,i,d;
    Roller.getPosPID(&p,&i,&d);
    mrbc_value ret = mrbc_array_new(vm, 3);
    mrbc_value rp = mrbc_fixnum_value((int)p);
    mrbc_value ri = mrbc_fixnum_value((int)i);
    mrbc_value rd = mrbc_fixnum_value((int)d);
    mrbc_array_set(&ret, 0, &rp);
    mrbc_array_set(&ret, 1, &ri);
    mrbc_array_set(&ret, 2, &rd);
    SET_RETURN(ret);
}

static void class_roller485_setpospid(mrb_vm *vm, mrb_value *v, int argc) {
    if (argc > 2) {
        uint32_t p = val_to_i(vm, v, GET_ARG(1), argc);
        uint32_t i = val_to_i(vm, v, GET_ARG(2), argc);
        uint32_t d = val_to_i(vm, v, GET_ARG(3), argc);
        Roller.setPosPID(p,i,d);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "pospid");
        SET_FALSE_RETURN();
    }
}

static void class_roller485_getspeedpid(mrb_vm *vm, mrb_value *v, int argc) {
    uint32_t p,i,d;
    Roller.getSpeedPID(&p,&i,&d);
    mrbc_value ret = mrbc_array_new(vm, 3);
    mrbc_value rp = mrbc_fixnum_value((int)p);
    mrbc_value ri = mrbc_fixnum_value((int)i);
    mrbc_value rd = mrbc_fixnum_value((int)d);
    mrbc_array_set(&ret, 0, &rp);
    mrbc_array_set(&ret, 1, &ri);
    mrbc_array_set(&ret, 2, &rd);
    SET_RETURN(ret);
}

static void class_roller485_setspeedpid(mrb_vm *vm, mrb_value *v, int argc) {
    if (argc > 2) {
        uint32_t p = val_to_i(vm, v, GET_ARG(1), argc);
        uint32_t i = val_to_i(vm, v, GET_ARG(2), argc);
        uint32_t d = val_to_i(vm, v, GET_ARG(3), argc);
        Roller.setSpeedPID(p,i,d);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "speedpid");
        SET_FALSE_RETURN();
    }
}

static void class_roller485_setdialcounter(mrb_vm *vm, mrb_value *v, int argc) {
    if (argc > 0) {
        int32_t counter = val_to_i(vm, v, GET_ARG(1), argc);
        Roller.setDialCounter(counter);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "counter");
        SET_FALSE_RETURN();
    }
}
static void class_roller485_getdialcounter(mrb_vm *vm, mrb_value *v, int argc) {
    int32_t counter = Roller.getDialCounter();
    SET_INT_RETURN(counter);
}

static void class_roller485_set_pos_maxcurrent(mrb_vm *vm, mrb_value *v, int argc) {
    if (argc > 0) {
        int32_t current = val_to_i(vm, v, GET_ARG(1), argc);
        Roller.setPosMaxCurrent(current);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "posmaxcurrent");
        SET_FALSE_RETURN();
    }
}

static void class_roller485_setRGBBrightness(mrb_vm *vm, mrb_value *v, int argc) {
    if (argc > 0) {
        uint8_t brightness = val_to_i(vm, v, GET_ARG(1), argc);
        Roller.setRGBBrightness(brightness);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "brightness");
        SET_FALSE_RETURN();
    }
}
static void class_roller485_getRGBBrightness(mrb_vm *vm, mrb_value *v, int argc) {
    int brightness = Roller.getRGBBrightness();
    SET_INT_RETURN(brightness);
}

static void class_roller485_set_RGBmode(mrb_vm *vm, mrb_value *v, int argc) {
    if (argc > 0) {
        int mode = val_to_i(vm, v, GET_ARG(1), argc);
        Roller.setRGBMode((roller_rgb_t) mode);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "rgbmode");
        SET_FALSE_RETURN();
    }
}
static void class_roller485_get_RGBmode(mrb_vm *vm, mrb_value *v, int argc) {
    int mode = Roller.getRGBMode();
    SET_INT_RETURN(mode);
}

static void class_roller485_set_RGBcolor(mrb_vm *vm, mrb_value *v, int argc) {
    if (argc > 0) {
        int color = val_to_i(vm, v, GET_ARG(1), argc);
        Roller.setRGB(color);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "rgbcolor");
        SET_FALSE_RETURN();
    }
}

static void class_roller485_get_outputstatus(mrb_vm *vm, mrb_value *v, int argc) {
    int status = Roller.getOutputStatus();
    SET_INT_RETURN(status);
}

static void class_roller485_get_sysstatus(mrb_vm *vm, mrb_value *v, int argc) {
    int status = Roller.getSysStatus();
    SET_INT_RETURN(status);
}

static void class_roller485_gettemp(mrb_vm *vm, mrb_value *v, int argc) {
    int temp = Roller.getTemp();
    SET_INT_RETURN(temp);
}

static void class_roller485_new(mrb_vm *vm, mrb_value *v, int argc) {
    mrbc_raise(vm, MRBC_CLASS(RuntimeError), "not yet implemented");
    SET_FALSE_RETURN();
}

void class_roller485_init(){
    if(!default_roller485_init()) {
        mrbc_set_const(mrbc_str_to_symid("Roller485"),&failed_object);
        return;
    }
    mrbc_class *roller485_class = mrbc_define_class(0,"Roller485", mrbc_class_object);
    mrbc_define_method(0, roller485_class, "new", class_roller485_new);
    mrbc_define_method(0, roller485_class, "calibrate", class_roller485_calibration);
    mrbc_define_method(0, roller485_class, "set_mode", class_roller485_setmode);
    mrbc_define_method(0, roller485_class, "get_mode", class_roller485_getmode);
    mrbc_define_method(0, roller485_class, "set_output", class_roller485_setoutput);

    mrbc_define_method(0, roller485_class, "set_current", class_roller485_setcurrent);
    mrbc_define_method(0, roller485_class, "get_current", class_roller485_getcurrent);
    mrbc_define_method(0, roller485_class, "get_current_readback", class_roller485_current_readback);

    mrbc_define_method(0, roller485_class, "set_pos", class_roller485_setpos);
    mrbc_define_method(0, roller485_class, "get_pos", class_roller485_getpos);
    mrbc_define_method(0, roller485_class, "get_pos_readback", class_roller485_getpos_readback);
    mrbc_define_method(0, roller485_class, "set_pos_maxcurrent", class_roller485_set_pos_maxcurrent);
    mrbc_define_method(0, roller485_class, "get_pos_pid", class_roller485_getpospid);
    mrbc_define_method(0, roller485_class, "set_pos_pid", class_roller485_setpospid);

    mrbc_define_method(0, roller485_class, "set_speed", class_roller485_setspeed);
    mrbc_define_method(0, roller485_class, "get_speed", class_roller485_getspeed);
    mrbc_define_method(0, roller485_class, "get_speed_readback", class_roller485_speed_readback);
    mrbc_define_method(0, roller485_class, "set_speed_maxcurrent", class_roller485_setspeedmaxcurrent);
    mrbc_define_method(0, roller485_class, "get_speed_pid", class_roller485_getspeedpid);
    mrbc_define_method(0, roller485_class, "set_speed_pid", class_roller485_setspeedpid);
    
    mrbc_define_method(0, roller485_class, "set_dial_counter", class_roller485_setdialcounter);
    mrbc_define_method(0, roller485_class, "get_dial_counter", class_roller485_getdialcounter);

    mrbc_define_method(0, roller485_class, "set_rgb_brightness", class_roller485_setRGBBrightness);
    mrbc_define_method(0, roller485_class, "get_rgb_brightness", class_roller485_getRGBBrightness);

    mrbc_define_method(0, roller485_class, "set_rgb_mode", class_roller485_set_RGBmode);
    mrbc_define_method(0, roller485_class, "get_rgb_mode", class_roller485_get_RGBmode);
    mrbc_define_method(0, roller485_class, "set_rgb", class_roller485_set_RGBcolor);

    mrbc_define_method(0, roller485_class, "get_output_status", class_roller485_get_outputstatus);

    mrbc_define_method(0, roller485_class, "get_sys_status", class_roller485_get_sysstatus);
    mrbc_define_method(0, roller485_class, "get_temperature", class_roller485_gettemp);
}

