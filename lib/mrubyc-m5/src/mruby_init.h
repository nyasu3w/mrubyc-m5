#include "my_mrubydef.h"

// configured init function

#include "c_m5.h"
#include "c_display_button.h"

#ifdef USE_SD_FUNCTION
#include "c_imu.h"
#endif
#ifdef USE_SD_FUNCTION
#include "c_sd.h"
#endif
#ifdef USE_CARDKB
#include "c_cardkb.h"
#endif
#ifdef USE_TOUCH
#include "c_touch.h"
#endif



void my_mrubyc_init(){
    class_m5_init();
    class_display_button_init();

#ifdef USE_CARDKB
    class_cardkb_init();
#else
    mrbc_set_const(mrbc_str_to_symid("CardKB"), &failed_object);
#endif
#ifdef USE_M5UNIFIED_IMU
    class_imu_init();
#else
    mrbc_set_const(mrbc_str_to_symid("IMU"), &failed_object);
#endif
#ifdef USE_SD_FUNCTION
    class_sd_init();  // define SD 
#else
    mrbc_set_const(mrbc_str_to_symid("SD"), &failed_object);
#endif
#ifdef USE_TOUCH
    class_touch_init();
#else
    mrbc_set_const(mrbc_str_to_symid("Touch"), &failed_object);
#endif // USE_TOUCH
}

