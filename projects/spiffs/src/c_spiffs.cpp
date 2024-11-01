#include <FS.h>
#include <SPIFFS.h>

#include "my_mrubydef.h"
#include "c_spiffs.h"

static void class_spiffs_open(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        const char* path = val_to_s(vm, v, GET_ARG(1),argc);
        auto mode = FILE_READ;
        if(argc>1){
            const char* m = val_to_s(vm, v, GET_ARG(2),argc);
            if(strcmp(m,"w")==0){
                mode = FILE_WRITE;
            } else if(strcmp(m,"a")==0){
                mode = FILE_APPEND;
            } else if(strcmp(m,"r")==0){
                mode = FILE_READ;
            }
        }

        if(SPIFFS.exists(path)){
            File f = SPIFFS.open(path, mode);
            if(!f){
                SET_FALSE_RETURN();
                return;
            }
            if(f.isDirectory()){
                f.close();
                SET_FALSE_RETURN();
                return;
            }
            mrbc_value spiffs = mrbc_instance_new(0, class_spiffs, sizeof(File*));
            *(File**) spiffs.instance->data = new File(f);
            SET_RETURN(spiffs);
            return;
        } 
    }
    SET_FALSE_RETURN();
}

void class_spiffs_init() {
    if(!SPIFFS.begin()){  // if failed
        Serial.println("SPIFFS.begin failed");
        mrbc_set_const(mrbc_str_to_symid("SPIFFS"),&failed_object);
        return;
    }
    mrbc_class mrbc_class *class_spiffs=mrbc_define_class(0,"SPIFFS", mrbc_class_object);
    mrbc_define_method(0, class_spiffs, "available?", true_return);

    mrbc_define_method(0, class_spiffs, "open", class_spiffs_open);
/*
    mrbc_define_method(0, class_spiffs, "read", class_spiffs_read);
    mrbc_define_method(0, class_spiffs, "close", class_spiffs_close);
    mrbc_define_method(0, class_spiffs, "exists?", class_spiffs_exists);
    mrbc_define_method(0, class_spiffs, "remove", class_spiffs_remove);
    mrbc_define_method(0, class_spiffs, "write", class_spiffs_write);
*/
}
