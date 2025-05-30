#include <FS.h>
#include <SPIFFS.h>

#include "my_mrubydef.h"
#include "c_file.h"
#include "c_spiffs.h"

#ifndef USE_FILE_CLASS
#error "USE_FILE_CLASS must be defined"
#endif


static void class_spiffs_open(mrb_vm *vm, mrb_value *v, int argc)
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
        if((path[0]=='/' && path[1]==0) || SPIFFS.exists(path)){  // ??
            File f = SPIFFS.open(path, mode);
            if(!f){
                SET_FALSE_RETURN();
                return;
            }
            mrbc_value file = mrbc_instance_new(0, class_file, sizeof(File*));
            *(File**) file.instance->data = new File(f);
            SET_RETURN(file);
            return;
        } 
    }
    SET_FALSE_RETURN();
}

static void class_spiffs_exists(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        const char* path = MRBC_ARG_S(1);
        if(SPIFFS.exists(path)){
            SET_TRUE_RETURN();
            return;
        }
    }
    SET_FALSE_RETURN();
}

static void class_spiffs_remove(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        const char* path = MRBC_ARG_S(1);
        if(SPIFFS.exists(path)){
            if(SPIFFS.remove(path)){
                SET_TRUE_RETURN();
                return;
            }
        }
    }
    SET_FALSE_RETURN();
}

void class_spiffs_init() {
    constexpr bool FORMAT_SPIFFS_IF_FAILED=true;
    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){  // if failed
        M5.Log.println("SPIFFS.begin failed");
        mrbc_set_const(mrbc_str_to_symid("SPIFFS"),&failed_object);
        return;
    }
    mrbc_class *class_spiffs=mrbc_define_class(0,"SPIFFS", mrbc_class_object);
    mrbc_define_method(0, class_spiffs, "available?", true_return);

    mrbc_define_method(0, class_spiffs, "open", class_spiffs_open);
    mrbc_define_method(0, class_spiffs, "exists?", class_spiffs_exists);
    mrbc_define_method(0, class_spiffs, "remove", class_spiffs_remove);

}
