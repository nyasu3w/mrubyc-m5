#include <M5Unified.h>
#include "my_mrubydef.h"
#include "c_sd.h"

#ifdef USE_SD_FUNCTION

static mrb_class *class_sd;

static void class_sd_open(mrb_vm *vm, mrb_value *v, int argc)
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

        if(SD.exists(path) || mode == FILE_WRITE){
            File f = SD.open(path, mode);
            if(f){
                mrbc_value sd = mrbc_instance_new(0, class_sd, sizeof(File*));
                *(File**) sd.instance->data = new File(f);
                SET_RETURN(sd);
                return;
            }
        } 
    }
    SET_FALSE_RETURN();
}

static void
class_sd_close(mrb_vm *vm, mrb_value *v, int argc)
{
    File *f = *(File**) v->instance->data;
    if(f){
        f->close();
        delete f;
        *(File**) v->instance->data = NULL;
    }
    SET_NIL_RETURN();
}

static void
class_sd_exists(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        const char* path = val_to_s(vm, v, GET_ARG(1),argc);
        if(SD.exists(path)){
            SET_TRUE_RETURN();
            return;
        }
    }
    SET_FALSE_RETURN();
    return;
}

static void
class_sd_read(mrb_vm *vm, mrb_value *v, int argc)
{
    File *f = *(File**) v->instance->data;
    if(f){
        if(argc>0){
            int len = val_to_i(vm, v, GET_ARG(1),argc);
            if(len>0){
                char *buf = (char*)mrbc_alloc(vm, len+1);
                if(buf){
                    int r = f->read((uint8_t*)buf, len);
                    buf[r] = '\0';
                    SET_RETURN(mrbc_string_new_cstr(vm, buf));
                    mrbc_raw_free(buf);
                    return;
                }
            }
        }
    }
    SET_NIL_RETURN();
}

static void
class_sd_remove(mrb_vm *vm, mrb_value *v, int argc)
{
    if(argc>0){
        const char* path = val_to_s(vm, v, GET_ARG(1),argc);
        if(SD.remove(path)){
            SET_TRUE_RETURN();
            return;
        }
    }
    SET_FALSE_RETURN();
    return;
}

static void
class_sd_write(mrb_vm *vm, mrb_value *v, int argc)
{
    File *f = *(File**) v->instance->data;
    if(f){
        int r=0;
        if(argc>0){
            if(v[1].tt == MRBC_TT_STRING){   // do not to use val_to_s for included '\0'
                const char* str = (const char*) v[1].string->data;
                uint16_t len = v[1].string->size;
                r = f->write((uint8_t*)str, len);
            } else {
                const char* str = val_to_s(vm, v, GET_ARG(1),argc);
                r = f->write((uint8_t*)str, strlen(str));
            }
            SET_INT_RETURN(r);
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
    class_sd = mrbc_define_class(0, "SD", mrbc_class_object);
    mrbc_define_method(0, class_sd, "available?", true_return);
    mrbc_define_method(0, class_sd, "open", class_sd_open);
    mrbc_define_method(0, class_sd, "read", class_sd_read);
    mrbc_define_method(0, class_sd, "close", class_sd_close);
    mrbc_define_method(0, class_sd, "exists?", class_sd_exists);
    mrbc_define_method(0, class_sd, "remove", class_sd_remove);
    mrbc_define_method(0, class_sd, "write", class_sd_write);
}

void class_sd_init(){
    if(SD.begin(GPIO_NUM_4, SPI, 15000000)){
        delay(100);
        class_sd_init_normal();  // define SD 
    } else {
        class_sd_init_failed();
    }  // for SD
}


#endif // USE_SD_FUNCTION