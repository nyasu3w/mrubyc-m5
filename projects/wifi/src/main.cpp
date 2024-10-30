
#include "M5Unified.h"

/* This configuration block shall be before including mruby_init.h */
//#define SUPPRESS_USEDEFS  // mandatory to configure: suppress default configuration
//#define USE_DISPLAY_GRAPHICS
//#define USE_TOUCH
/* configration block end*/

#include "mruby_init.h"
#include "c_wifi.h"

#define mrubycode test // codename generated by mrbc from test.rb

#define MEMSIZE (1024*50)
static uint8_t mempool[MEMSIZE];
extern const uint8_t mrubycode[]; 

static struct VM *vm;
unsigned char buff[10*1024];
int buff_ptr=0;


void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.setTextSize(2);

  Serial.println("start");

  mrbc_init(mempool, MEMSIZE);
  my_mrubyc_init();
  class_wifi_init();
}

void loop(){
  auto tcb = mrbc_create_task( mrubycode, 0 );
  if(NULL == tcb){
    M5.Display.print("setup error");
    Serial.println("mrbc_create_task error");  // may not be printed
    return;
  }
  mrbc_run();
  mrbc_delete_task(tcb);

  delay(2000);
}
