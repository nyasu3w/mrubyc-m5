
#include <M5Unified.h>
#include "mruby_init.h"

#define mrubycode test // codename generated by mrbc from demo.rb

#define MEMSIZE (1024*50)
static uint8_t mempool[MEMSIZE];
extern const uint8_t mrubycode[];

static struct VM *vm;
unsigned char buff[10*1024];
int buff_ptr=0;

void show_exception_screen(){
  M5.Lcd.fillScreen(TFT_RED);
  int w = M5.Lcd.width();
  int h = M5.Lcd.height();
  int sz=(w>200)? 2 : 1;
  for(int i=4; i<18*sz; i++){
    M5.Lcd.fillRect(0, h/2-i, w, i*2, TFT_BLACK);
    M5.Lcd.drawRect(0, h/2-i+2, w, i*2-4, TFT_YELLOW);
    M5.Display.waitDisplay();
  }
  M5.Lcd.setTextSize(sz);
  M5.Lcd.setFont(nullptr);
  M5.Lcd.setCursor(w/2-26*sz, h/2-12*sz);
  M5.Lcd.setTextColor(TFT_YELLOW);
  M5.Lcd.println("EXCEPTION");
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setCursor(w/2-30*sz, M5.Lcd.getCursorY());
  M5.Lcd.println("See Serial.");
  M5.Lcd.setCursor(w/2-38*sz, M5.Lcd.getCursorY());
  M5.Lcd.println("Press to exit");
  delay(500);
  M5.update(); M5.update(); //clear touch count
  while(M5.Touch.getCount()==0 || M5.BtnA.wasPressed()){
    delay(100);
    M5.update();
  }
}


void m_setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.setTextSize(2);

  M5.Log.println("start");

  mrbc_init(mempool, MEMSIZE);
  my_mrubyc_init();
}

void m_loop(){
  auto tcb = mrbc_create_task( mrubycode, 0 );
  if(NULL == tcb){
    M5.Display.print("setup error");
    M5.Log.println("mrbc_create_task error");  // may not be printed
    return;
  }
  int ret = mrbc_run();
  if(ret == 2 && M5.getBoard()<lgfx::boards::board_M5AtomLite){
    show_exception_screen();
  }
  mrbc_delete_task(tcb);
  M5.Log.println("mrbc done");
}
