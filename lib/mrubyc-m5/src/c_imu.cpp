#include <M5Unified.h>
#include "my_mrubydef.h"
#include "c_imu.h"

#ifdef USE_IMU_FUNCTION

static void
class_imu_get_accel(mrb_vm *vm, mrb_value *v, int argc)
{
    float accel[3];
    M5.Imu.getAccel(&accel[0], &accel[1], &accel[2]);
    mrbc_value ret = mrbc_array_new(vm, 3);
    mrbc_value x = mrbc_float_value(vm,accel[0]);
    mrbc_value y = mrbc_float_value(vm,accel[1]);
    mrbc_value z = mrbc_float_value(vm,accel[2]);
    mrbc_array_set(&ret, 0, &x);
    mrbc_array_set(&ret, 1, &y);
    mrbc_array_set(&ret, 2, &z);
    SET_RETURN(ret);
}

static void
class_imu_get_gyro(mrb_vm *vm, mrb_value *v, int argc)
{
    float gyro[3];
    M5.Imu.getGyro(&gyro[0], &gyro[1], &gyro[2]);
    mrbc_value ret = mrbc_array_new(vm, 3);
    mrbc_value x = mrbc_float_value(vm,gyro[0]);
    mrbc_value y = mrbc_float_value(vm,gyro[1]);
    mrbc_value z = mrbc_float_value(vm,gyro[2]);
    mrbc_array_set(&ret, 0, &x);
    mrbc_array_set(&ret, 1, &y);
    mrbc_array_set(&ret, 2, &z);
    SET_RETURN(ret);
}

/*  not implemented on M5Unified side
static void
class_imu_get_ahrs(mrb_vm *vm, mrb_value *v, int argc)
{
    float ahrs[3];
    M5.Imu.getAhrsData(&ahrs[0], &ahrs[1], &ahrs[2]);
    mrbc_value ret = mrbc_array_new(vm, 3);
    mrbc_value x = mrbc_float_value(vm,ahrs[0]);
    mrbc_value y = mrbc_float_value(vm,ahrs[1]);
    mrbc_value z = mrbc_float_value(vm,ahrs[2]);
    mrbc_array_set(&ret, 0, &x);
    mrbc_array_set(&ret, 1, &y);
    mrbc_array_set(&ret, 2, &z);
    SET_RETURN(ret);
}
*/

static void class_imu_get_temp(mrb_vm *vm, mrb_value *v, int argc)
{
    float temp;
    if(M5.Imu.getTemp(&temp)){
        SET_FLOAT_RETURN(temp);
    } else {
        SET_FALSE_RETURN();
    }
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
    mrbc_define_method(0, class_imu, "gyro", class_imu_get_gyro);
//    mrbc_define_method(0, class_imu, "ahrs", class_imu_get_ahrs);
    mrbc_define_method(0, class_imu, "temp", class_imu_get_temp);
}

void class_imu_init(){
    if(M5.Imu.isEnabled()){
        class_imu_init_normal();
    } else {
        class_imu_init_failed();
    }
}


#endif // USE_IMU_FUNCTION
