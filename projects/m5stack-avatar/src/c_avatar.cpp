#include <M5Unified.h>
#include <Avatar.h>

#include "my_mrubydef.h"
#include "c_avatar.h"


static void class_avatar_new(mrb_vm *vm, mrb_value *v, int argc)
{
    v[0] = mrbc_instance_new(vm, v[0].cls, sizeof(m5avatar::Avatar));
    mrbc_instance_call_initialize( vm, v, argc );
}  

static void class_avatar_initialize(mrb_vm *vm, mrb_value *v, int argc){
    m5avatar::Avatar* avatar = new m5avatar::Avatar();
    *(m5avatar::Avatar**) v->instance->data = avatar;

    avatar->setSpeechFont(&fonts::lgfxJapanGothic_12);
}
static void class_avatar_set(mrb_vm *vm, mrb_value *v, int argc){
    m5avatar::Avatar *avatar = get_checked_data(m5avatar::Avatar, vm, v);
    if(argc == 2){
            int x = MRBC_ARG_I(1);
            int y = MRBC_ARG_I(2);
            avatar->setPosition(x, y);
    } else {
        mrbc_raise( vm, MRBC_CLASS(TypeError), "Invalid argument type");
        SET_NIL_RETURN();
    }
}

static void class_avatar_scale(mrb_vm *vm, mrb_value *v, int argc){
    m5avatar::Avatar *avatar = get_checked_data(m5avatar::Avatar, vm, v);
    if(argc == 1){
        float scale = MRBC_ARG_F(1);
        avatar->setScale(scale);
    }
    SET_NIL_RETURN();
}

static void class_avatar_start(mrb_vm *vm, mrb_value *v, int argc){
    m5avatar::Avatar *avatar = get_checked_data(m5avatar::Avatar, vm, v);
    avatar->start();
}

static void class_avatar_set_speech_text(mrb_vm *vm, mrb_value *v, int argc){
    m5avatar::Avatar *avatar = get_checked_data(m5avatar::Avatar, vm, v);
    if(argc == 1){
        const char* text = MRBC_ARG_S(1);
        avatar->setSpeechText(text);
    }
}

static void class_avatar_set_rotation(mrb_vm *vm, mrb_value *v, int argc){
    m5avatar::Avatar *avatar = get_checked_data(m5avatar::Avatar, vm, v);
    if(argc == 1){
        float radian = MRBC_ARG_F(1);
        avatar->setRotation(radian);
    }
    SET_NIL_RETURN();
}

static void class_avatar_set_expression(mrb_vm *vm, mrb_value *v, int argc){
    m5avatar::Avatar *avatar = get_checked_data(m5avatar::Avatar, vm, v);
    if(argc == 1){
        int exp = MRBC_ARG_I(1);
        avatar->setExpression((m5avatar::Expression)exp);
    }
}

static void class_avatar_destroy(mrb_vm *vm, mrb_value *v, int argc){
    m5avatar::Avatar *avatar = get_checked_data(m5avatar::Avatar, vm, v);
    avatar->stop();
    delete avatar;
    put_null_data(v);
}

void class_avatar_init(){
    mrb_class *class_avatar;
    class_avatar = mrbc_define_class(0, "Avatar", mrbc_class_object);
    mrbc_define_method(0, class_avatar, "new", class_avatar_new);
    mrbc_define_method(0, class_avatar, "initialize", class_avatar_initialize);
    mrbc_define_method(0, class_avatar, "set_position", class_avatar_set);
    mrbc_define_method(0, class_avatar, "set_speech_text", class_avatar_set_speech_text);
    mrbc_define_method(0, class_avatar, "set_rotation", class_avatar_set_rotation);
    mrbc_define_method(0, class_avatar, "set_expression", class_avatar_set_expression);

    mrbc_define_method(0, class_avatar, "scale=", class_avatar_scale);
    mrbc_define_method(0, class_avatar, "start", class_avatar_start);
    mrbc_define_method(0, class_avatar, "destroy", class_avatar_destroy);
}
