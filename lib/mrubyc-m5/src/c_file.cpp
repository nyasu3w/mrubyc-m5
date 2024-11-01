#include "my_mrubydef.h"

#ifdef USE_FILE_CLASS

#include "c_file.h"

mrb_class *class_file;

static void
class_file_read(mrb_vm *vm, mrb_value *v, int argc)
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
class_file_write(mrb_vm *vm, mrb_value *v, int argc)
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

static void
class_file_close(mrb_vm *vm, mrb_value *v, int argc)
{
    File *f = *(File**) v->instance->data;
    if(f){
        f->close();
        delete f;
        *(File**) v->instance->data = NULL;
    }
    SET_NIL_RETURN();
}


void class_file_init()
{
    class_file = mrbc_define_class(0, "File", mrbc_class_object);
    mrbc_define_method(0, class_file, "read", class_file_read);
    mrbc_define_method(0, class_file, "close", class_file_close);
    mrbc_define_method(0, class_file, "write", class_file_write);
}

#endif // USE_FILE_FUNCTION
