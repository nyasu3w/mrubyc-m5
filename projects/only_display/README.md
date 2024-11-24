# only_display
This is a sample to reduce features.

## point to see
It is based on total_demo projects, but many functions can be disabled.
Edit `lib/mrubyc-m5/src/libconfig.h` as below to disable useless functions by comment out the define macro.

```
#ifndef A_LIBCONFIG_H
#define A_LIBCONFIG_H

//* ******* default configuration block ****** */

//#define FULL_BUTTONS    // to enable BtnEXT and BtnPWR
#define USE_DISPLAY_GRAPHICS  // Display to support graphics functions
  //#define USE_IMU_FUNCTION  // IMU to support M5Unified IMU functions
  //#define USE_SD_FUNCTION  // to support SD functions
  //#define USE_FILE_CLASS  // to support File functions
#define USE_TOUCH // to support Touch functions
  //#define USE_SPEAKER // to support Speaker functions
  //#define USE_CANVAS // to support Canvas functions
  //#define USE_MULTIDISPLAY // to support multiple displays
  // #define USE_ESPNOW // to support ESP Now

  // #define USE_TEMPORAL_RANDOM_FUNCTION  // it seems that mruby/c does not have random number function
//* ******* default configuration block end****** */

#endif // A_LIBCONFIG_H




```

