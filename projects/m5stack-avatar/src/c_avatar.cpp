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
    m5avatar::Avatar *avatar = *(m5avatar::Avatar**)v->instance->data;
    if(argc == 2){
            int x = val_to_i(vm, v, GET_ARG(1), argc);
            int y = val_to_i(vm, v, GET_ARG(2), argc);
            avatar->setPosition(x, y);
    } else {
        mrbc_raise( vm, MRBC_CLASS(TypeError), "Invalid argument type");
        SET_NIL_RETURN();
    }
}

static void class_avatar_scale(mrb_vm *vm, mrb_value *v, int argc){
    m5avatar::Avatar *avatar = *(m5avatar::Avatar**)v->instance->data;
    if(argc == 1){
        float scale = val_to_f(vm, v, GET_ARG(1), argc);
        avatar->setScale(scale);
    }
    SET_NIL_RETURN();
}

static void class_avatar_start(mrb_vm *vm, mrb_value *v, int argc){
    m5avatar::Avatar *avatar = *(m5avatar::Avatar**)v->instance->data;
    avatar->start();
}

static void class_avatar_set_speech_text(mrb_vm *vm, mrb_value *v, int argc){
    m5avatar::Avatar *avatar = *(m5avatar::Avatar**)v->instance->data;
    if(argc == 1){
        const char* text = val_to_s(vm, v, GET_ARG(1), argc);
        avatar->setSpeechText(text);
    }
}

static void class_avatar_set_rotation(mrb_vm *vm, mrb_value *v, int argc){
    m5avatar::Avatar *avatar = *(m5avatar::Avatar**)v->instance->data;
    if(argc == 1){
        float radian = val_to_f(vm, v, GET_ARG(1), argc);
        avatar->setRotation(radian);
    }
    SET_NIL_RETURN();
}

static void class_avatar_set_expression(mrb_vm *vm, mrb_value *v, int argc){
    m5avatar::Avatar *avatar = *(m5avatar::Avatar**)v->instance->data;
    if(argc == 1){
        int exp = val_to_i(vm, v, GET_ARG(1), argc);
        avatar->setExpression((m5avatar::Expression)exp);
    }
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
}
