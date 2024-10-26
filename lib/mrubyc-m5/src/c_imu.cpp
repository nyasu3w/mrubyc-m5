#include <M5Unified.h>
#include "my_mrubydef.h"
#include "c_imu.h"

#ifdef USE_M5UNIFIED_IMU

static void
class_imu_get_accel(mrb_vm *vm, mrb_value *v, int argc)
{
    float accel[3];
    M5.Imu.getAccelData(&accel[0], &accel[1], &accel[2]);
    mrbc_value ret = mrbc_array_new(vm, 3);
    mrbc_value x = mrbc_float_value(vm,accel[0]);
    mrbc_value y = mrbc_float_value(vm,accel[1]);
    mrbc_value z = mrbc_float_value(vm,accel[2]);
    mrbc_array_set(&ret, 0, &x);
    mrbc_array_set(&ret, 1, &y);
    mrbc_array_set(&ret, 2, &z);
    SET_RETURN(ret);
}

static void class_imu_init_failed(){
    mrbc_set_const(mrbc_str_to_symid("IMU"),&failed_object);
}


static void class_imu_init_normal()
{
    mrb_class *class_imu;
    class_imu = mrbc_define_class(0, "IMU", mrbc_class_object);
    mrbc_define_method(0, class_imu, "available?", true_return);
    mrbc_define_method(0, class_imu, "accel", class_imu_get_accel);
}

void class_imu_init(){
    if(M5.Imu.isEnabled()){
        class_imu_init_normal();
    } else {
        class_imu_init_failed();
    }
}


#endif // USE_M5UNIFIED_IMU