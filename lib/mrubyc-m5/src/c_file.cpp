#include "my_mrubydef.h"

#ifdef USE_FILE_CLASS

#include "c_file.h"

mrb_class *class_file;

static void
class_file_read(mrb_vm *vm, mrb_value *v, int argc)
{
    File *f = get_checked_data(File, vm, v);
    if(f){
        int len = 255;
        if(argc>0){
            len = MRBC_ARG_I(1);
        } 

        if(len>0){
            char *buf = (char*)mrbc_alloc(vm, len+1);
            if(buf){
                int r = f->read((uint8_t*)buf, len);
                SET_RETURN(mrbc_string_new(vm, buf,r));
                mrbc_raw_free(buf);
                return;
            }
        }
    }
    SET_NIL_RETURN();
}

static void
class_file_write(mrb_vm *vm, mrb_value *v, int argc)
{
    File *f = get_checked_data(File, vm, v);
    if(f){
        int r=0;
        if(argc>0){
            if(v[1].tt == MRBC_TT_STRING){   // do not to use val_to_s for included '\0'
                const char* str = (const char*) v[1].string->data;
                uint16_t len = v[1].string->size;
                r = f->write((uint8_t*)str, len);
            } else {
                const char* str = MRBC_ARG_S(1);
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
    File *f = get_checked_data(File, vm, v);
    if(f){
        f->close();
        delete f;
        *(File**) v->instance->data = NULL;
    }
    SET_NIL_RETURN();
}

static void
class_file_available(mrb_vm *vm, mrb_value *v, int argc)
{
    File *f = get_checked_data(File, vm, v);
    if(f->available()){
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void class_file_is_directory(mrb_vm *vm, mrb_value *v, int argc)
{
    File *f = get_checked_data(File, vm, v);
    if(f->isDirectory()){
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void class_file_size(mrb_vm *vm, mrb_value *v, int argc)
{
    File *f = get_checked_data(File, vm, v);
    SET_INT_RETURN(f->size());
}

static void class_file_open_next_file(mrb_vm *vm, mrb_value *v, int argc)
{
    File *f = get_checked_data(File, vm, v);
    if(!f->isDirectory()){
        mrbc_raise(vm, MRBC_CLASS(ArgumentError),"not directory");
        return;
    }
    File next = f->openNextFile();
    if(next){
        mrbc_value file = mrbc_instance_new(0, class_file, sizeof(File*));
        *(File**) file.instance->data = new File(next);
        SET_RETURN(file);
        return;
    } 
    SET_NIL_RETURN();
}

static void class_file_name(mrb_vm *vm, mrb_value *v, int argc)
{
    File *f = get_checked_data(File, vm, v);
    SET_RETURN(mrbc_string_new_cstr(vm, f->name()));
}
static void class_file_path(mrb_vm *vm, mrb_value *v, int argc)
{
    File *f = get_checked_data(File, vm, v);
    SET_RETURN(mrbc_string_new_cstr(vm, f->path()));
}

static void
class_file_destroy(mrb_vm *vm, mrb_value *v, int argc)
{
    File *f = get_checked_data(File, vm, v);
    if(f){
        f->close();
        delete f;
        put_null_data(v);
        SET_TRUE_RETURN();
    }
}



void class_file_init()
{
    class_file = mrbc_define_class(0, "File", mrbc_class_object);
    mrbc_define_method(0, class_file, "read", class_file_read);
    mrbc_define_method(0, class_file, "close", class_file_close);
    mrbc_define_method(0, class_file, "write", class_file_write);
    mrbc_define_method(0, class_file, "available?", class_file_available);
    mrbc_define_method(0, class_file, "is_directory?", class_file_is_directory);
    mrbc_define_method(0, class_file, "size", class_file_size);
    mrbc_define_method(0, class_file, "open_next_file", class_file_open_next_file);
    mrbc_define_method(0, class_file, "name", class_file_name);
    mrbc_define_method(0, class_file, "path", class_file_path);
    mrbc_define_method(0, class_file, "destroy", class_file_destroy);
}

#endif // USE_FILE_FUNCTION
