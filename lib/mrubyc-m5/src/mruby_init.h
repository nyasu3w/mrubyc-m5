//
// This file can be included only once from main file.
//

#include "my_mrubydef.h"

// configured init function

#ifndef MY_MRUBYC_INIT
#define MY_MRUBYC_INIT

#include "c_m5.h"
#include "c_utils.h"
#include "c_display_button.h"

#ifdef USE_DISPLAY_GRAPHICS
#include "c_font.h"
#endif
#ifdef USE_SD_FUNCTION
#include "c_imu.h"
#endif
#ifdef USE_SD_FUNCTION
#include "c_sd.h"
#endif
#ifdef USE_FILE_CLASS
#include "c_file.h"
#endif
#ifdef USE_TOUCH
#include "c_touch.h"
#endif
#ifdef USE_SPEAKER
#include "c_speaker.h"
#endif
#ifdef USE_CANVAS
#include "c_canvas.h"
#endif
#ifdef USE_ESPNOW
#include "c_espnow.h"
#endif

void my_mrubyc_init(){
    class_m5_init();
    class_display_button_init();
    class_utils_init();

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
#ifdef USE_FILE_CLASS
    class_file_init();  // define SD 
#else
    mrbc_set_const(mrbc_str_to_symid("File"), &failed_object);
#endif
#ifdef USE_TOUCH
    class_touch_init();
#else
    mrbc_set_const(mrbc_str_to_symid("Touch"), &failed_object);
#endif // USE_TOUCH
#ifdef USE_SPEAKER
    class_speaker_init();
#else
    mrbc_set_const(mrbc_str_to_symid("Speaker"), &failed_object);
#endif // USE_SPEAKER
#ifdef USE_CANVAS
    class_canvas_init();
#else
    mrbc_set_const(mrbc_str_to_symid("Canvas"), &failed_object);
#endif // USE_CANVAS
#ifdef USE_DISPLAY_GRAPHICS
    class_font_init();
#else
    mrbc_set_const(mrbc_str_to_symid("Font"), &failed_object);
#endif // USE_DISPLAY_GRAPHICS 
#ifdef USE_ESPNOW
    class_espnow_init();
#else
    mrbc_set_const(mrbc_str_to_symid("EspNow"), &failed_object);
#endif // USE_ESPNOW

}

#endif // MY_MRUBYC_INIT