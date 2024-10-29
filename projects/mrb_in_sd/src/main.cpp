
#include "M5Unified.h"
#include <Wire.h>
#include <vector>

/* This configuration block shall be before including mruby_init.h */
/* write nothing to apply the default configuration*/
/* configration block end*/
#include "mruby_init.h"

#define MEMSIZE (1024*50)
static uint8_t mempool[MEMSIZE];
uint8_t *mrubycode;

static struct VM *vm;
unsigned char buff[10*1024];
int buff_ptr=0;

constexpr int menu_max_items=5;
constexpr int menu_item_height=30;
constexpr int menu_off_x=30;
constexpr int menu_off_y=40;

void draw_menu(unsigned int start,std::vector<String> items, unsigned int focused) {
//    Serial.printf("menudraw: start: %d, focused: %d\n", start, focused);
    for(uint8_t i=0; i<menu_max_items; i++){
        if(i+start >= items.size()) break;
        if(i == focused-start){
            M5.Display.fillRect(menu_off_x-5, menu_off_y + menu_item_height*i-5, 320-menu_off_x*2+10, menu_item_height, TFT_BLUE);
            M5.Display.setTextColor(TFT_YELLOW);
        } else {
            M5.Display.fillRect(menu_off_x-5, menu_off_y + menu_item_height*i-5, 320-menu_off_x*2+10, menu_item_height, TFT_BLACK);
            M5.Display.setTextColor(TFT_WHITE);
        }
        M5.Display.setCursor(menu_off_x, menu_off_y + menu_item_height*i);
        M5.Display.print(items[i+start]);
    }   
}

const char* select_menu(std::vector<String> mrbfiles){
    constexpr int menu_bottom_y=210;
    M5.Display.clear();
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setTextSize(3);
    M5.Display.setCursor(0, 0);
    M5.Display.println("Select a file");
    M5.Display.setCursor(10, menu_bottom_y);
    M5.Display.setTextColor(TFT_BLACK,TFT_YELLOW);
    M5.Display.print("  UP ");
    M5.Display.setCursor(110, menu_bottom_y);
    M5.Display.setTextColor(TFT_BLACK,TFT_YELLOW);
    M5.Display.print(" SEL ");
    M5.Display.setCursor(210, menu_bottom_y);
    M5.Display.setTextColor(TFT_BLACK,TFT_YELLOW);
    M5.Display.print(" DWN ");

    unsigned int start=0; 
    int focused=0;  // not unsigned
    draw_menu(start, mrbfiles, focused);

    while(true){
        M5.update();

        typedef enum {NONE,M_UP,M_SEL,M_DOWN } TOUCHACTION;
        int touch_action = NONE;
        if(M5.Touch.getCount()>0){
            auto detail = M5.Touch.getDetail(0);
            int dy = detail.y - menu_off_y;
            if(0 < detail.y  && dy < menu_item_height*menu_max_items && detail.isHolding()){
                int new_focused = dy / menu_item_height + start;
                if(new_focused!=focused && new_focused < mrbfiles.size()){
                    focused = new_focused;
                    draw_menu(start, mrbfiles, focused);
                }
                continue;
            } else if(detail.wasClicked()){
                if(detail.y < menu_bottom_y) {
                    draw_menu(start, mrbfiles, focused);
                    int new_focused = dy / menu_item_height + start;
                    if(new_focused >= mrbfiles.size()) {
                        // do nothing by clicking out of menu
                    } else {
                        focused = new_focused;
                        touch_action = TOUCHACTION::M_SEL;
                    }
                } else {
                    if(detail.x < 100){
                        touch_action = TOUCHACTION::M_UP;
                    } else if(detail.x < 200){
                        touch_action = TOUCHACTION::M_SEL;
                    } else {
                        touch_action = TOUCHACTION::M_DOWN;
                    }
                }
            }
        }

        if(touch_action==TOUCHACTION::M_UP || M5.BtnA.wasPressed()){
            focused = focused - 1;
            if(focused < 0) focused = 0;
            if(focused <start) start = focused;
            draw_menu(start, mrbfiles, focused);
        } else if(touch_action==TOUCHACTION::M_SEL || M5.BtnB.wasPressed()){
            break;
        } else if(touch_action==TOUCHACTION::M_DOWN || M5.BtnC.wasPressed()){
            focused = focused + 1;
            if(focused >= mrbfiles.size()) focused = mrbfiles.size()-1;
            if(focused >= start+menu_max_items) start = focused - menu_max_items + 1;
            draw_menu(start, mrbfiles, focused);
        } 
        delay(50); // the last of while loop
    }
    delay(100);
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_WHITE,TFT_BLACK);
    M5.update();
    return mrbfiles[focused].c_str();
}

const char* sd_file_selector(){
    File d=SD.open("/mrubyc");
    std::vector<String> mrb_files;
    while(true){
        File entry = d.openNextFile();
        if(!entry) break;
        if(entry.isDirectory()) continue;

        String name = entry.name();
        if(name.endsWith(".mrb")){
            Serial.println(name);
            mrb_files.push_back(name);
        }
    }

    return (String("/mrubyc/") + select_menu(mrb_files)).c_str();
}

unsigned int sd_file_loader(const char* path){
    File file = SD.open(path);
    if(!file){
        Serial.println("file open error");
        return 0;
    }
    int size = file.size();

    mrubycode = (uint8_t*)malloc(size);
    if(mrubycode == NULL){
        Serial.println("malloc error");
        return 0;
    }
    file.read(mrubycode, size);
    file.close();
    return size;
}

void stop4ever(){
    M5.Display.clear();
    M5.Display.setTextColor(TFT_RED);
    M5.Display.println("mrb load error. ");
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.println("System stops forever");
    M5.Display.println("reset to push BtnA/B/C");
    while(1){
        M5.update();
        if(M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()){
            break;
        }
        delay(100);
    }
    ESP.restart();
}


void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    M5.Display.setTextSize(2);
    Serial.begin(115200);
    Wire.begin(); // for cardkb
    Serial.println("start");

    if(SD.begin(GPIO_NUM_4, SPI, 15000000)){
        const char* load_file = sd_file_selector();
        if(load_file == NULL){
            Serial.println("file select error");
            stop4ever();
        }
        Serial.printf("load_file: %s\n", const_cast<char*>(load_file));
        unsigned int r = sd_file_loader(load_file);
        if (r==0) {
            Serial.println("load error");
            stop4ever();
        }
    } else {
        Serial.println("SD mount failed");
        stop4ever();
    }
    Serial.println("mrb load done");
    mrbc_init(mempool, MEMSIZE);
    my_mrubyc_init();
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
    Serial.println("mrbc done");

    delay(2000);
}
