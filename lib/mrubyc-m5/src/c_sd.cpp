#include <M5Unified.h>
#include "my_mrubydef.h"
#include "c_sd.h"
#include "c_file.h"

#ifdef USE_SD_FUNCTION

#ifndef USE_FILE_CLASS
#error "USE_FILE_CLASS must be defined to use USE_SD_FUNCTION"
#endif



static void class_sd_open(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        const char* path = MRBC_ARG_S(1);
        auto mode = FILE_READ;
        if(argc>1){
            const char* m = MRBC_ARG_S(2);
            if(strcmp(m,"w")==0){
                mode = FILE_WRITE;
            } else if(strcmp(m,"a")==0){
                mode = FILE_APPEND;
            } else if(strcmp(m,"r")==0){
                mode = FILE_READ;
            }
        }

        if(SD.exists(path) || mode == FILE_WRITE){
            File f = SD.open(path, mode);
            if(f){
                mrbc_value sd = mrbc_instance_new(0, class_file, sizeof(File*));
                *(File**) sd.instance->data = new File(f);

                SET_RETURN(sd);
                return;
            }
        } 
    }
    SET_FALSE_RETURN();
}


static void
class_sd_exists(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        const char* path = MRBC_ARG_S(1);
        if(SD.exists(path)){
            SET_TRUE_RETURN();
            return;
        }
    }
    SET_FALSE_RETURN();
    return;
}


static void
class_sd_remove(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        const char* path = MRBC_ARG_S(1);
        if(SD.remove(path)){
            SET_TRUE_RETURN();
            return;
        }
    }
    SET_FALSE_RETURN();
    return;
}


static void class_sd_init_failed(){
    mrbc_set_const(mrbc_str_to_symid("SD"),&failed_object);
}

static void class_sd_init_normal()
{
    mrb_class *class_sd = mrbc_define_class(0, "SD", mrbc_class_object);
    mrbc_define_method(0, class_sd, "available?", true_return);
    mrbc_define_method(0, class_sd, "open", class_sd_open);
    mrbc_define_method(0, class_sd, "exists?", class_sd_exists);
    mrbc_define_method(0, class_sd, "remove", class_sd_remove);
}

int get_sdcard_gpio(){ 
    int sdcard_gpio[] = {
// unknown, M5Stack, M5StackCore2, M5StickC, M5StickCPlus
        -1, GPIO_NUM_4, GPIO_NUM_4,-1,-1,
// M5StickCPlus2, M5StackCoreInk, M5Paper,M5Tough,M5Station
        -1, -1, GPIO_NUM_4, GPIO_NUM_4,-1,
// M5StackCoreS3,M5AtomS3,M5Dial,M5DinMeter,M5Cardputer,
        GPIO_NUM_4,-1,-1,-1,-GPIO_NUM_12,
// M5AirQ,M5VAMeter,M5StackCoreS3SE,M5AtomS3R,
        -1,-1,GPIO_NUM_4,-1,
//board=139: M5CAPSULE:GPIO_NUM_11
    };

    int board = M5.getBoard();
    if(board==139){  //M5Capsule
        return GPIO_NUM_11;
    } else if(0 < board || board < sizeof(sdcard_gpio)/sizeof(sdcard_gpio[0])){
            return sdcard_gpio[board];
    }
    return -1;
}

void class_sd_init(){
    int gpio = get_sdcard_gpio();
    if(gpio>=0 && SD.begin(gpio, SPI)){
        delay(100);
        class_sd_init_normal();  // define SD 
    } else {
        class_sd_init_failed();
    }  // for SD
}


#endif // USE_SD_FUNCTION