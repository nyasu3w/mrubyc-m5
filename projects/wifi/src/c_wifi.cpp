#include <M5Unified.h>
#include <WiFi.h>

#include "my_mrubydef.h"
#include "c_wifi.h"



static void c_wifi_begin(mrb_vm *vm, mrb_value *v, int argc) {
    if(argc < 2){
        auto r = WiFi.begin();
        SET_INT_RETURN(r);
        return;
    }
    const char* ssid = val_to_s(vm,v,GET_ARG(1),argc);
    const char* password = val_to_s(vm,v,GET_ARG(2),argc);

    auto r = WiFi.begin(ssid, password);
    SET_INT_RETURN(r);
}

static void c_wifi_status(mrb_vm *vm, mrb_value *v, int argc) {
    auto r = WiFi.status();
    SET_INT_RETURN(r);
}

static void c_wifi_wait_for_connect(mrb_vm *vm, mrb_value *v, int argc) {
    uint8_t r;
    if(argc<1){
        r = WiFi.waitForConnectResult();
    }else {
        auto timeout = val_to_i(vm,v,GET_ARG(1),argc);
        r = WiFi.waitForConnectResult(timeout);
    }
    SET_INT_RETURN(r);
}

static void c_wifi_mode(mrb_vm *vm, mrb_value *v, int argc) {
    auto r = WiFi.getMode();
    SET_INT_RETURN(r);
    return;
}

static void c_wifi_set_mode(mrb_vm *vm, mrb_value *v, int argc) {
    if(argc < 1){
        return;
    }
    auto mode = (wifi_mode_t) val_to_i(vm,v,GET_ARG(1),argc);
    auto r = WiFi.mode(mode);
    SET_INT_RETURN(r);
    return;
}

static void c_wifi_disconnect(mrb_vm *vm, mrb_value *v, int argc) {
    if(argc < 1){
        auto r = WiFi.disconnect();
        SET_INT_RETURN(r);
        return;
    }
    auto r = WiFi.disconnect((bool) val_to_i(vm,v,GET_ARG(1),argc));
    SET_INT_RETURN(r);
    return;
}

static void c_wifi_softap(mrb_vm *vm, mrb_value *v, int argc) {
    if(argc < 2){
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "need 2 arguments");
        return;
    } else if(argc <= 3){
        // optional arg for softAP: channel,ssid_hidden,max_connection
    }
    const char* ssid = val_to_s(vm,v,GET_ARG(1),argc);
    const char* password = val_to_s(vm,v,GET_ARG(2),argc);
    auto r = WiFi.softAP(ssid, password);
    SET_INT_RETURN(r);
    return;
}

#ifdef USE_HTTPCLIENT
#include <HTTPClient.h>

mrb_class *class_httpclient;

static void c_httpclient_new(mrb_vm *vm, mrb_value *v, int argc) {
    v[0] = mrbc_instance_new(0, class_httpclient, sizeof(HTTPClient*));
    mrbc_instance_call_initialize( vm, v, argc );
}

static void c_httpclient_initialize(mrb_vm *vm, mrb_value *v, int argc) {
    if(argc < 1){
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "need 1 argument");
        return;
    }
    const char* url = val_to_s(vm,v,GET_ARG(1),argc);
    auto client = *(HTTPClient**) v->instance->data = new HTTPClient();
    client->begin(url);
}

static void c_httpclient_get(mrb_vm *vm, mrb_value *v, int argc) {
    auto client = *(HTTPClient**) v->instance->data;

    auto r = client->GET();
    SET_INT_RETURN(r);
    return;
}

static void c_httpclient_get_response(mrb_vm *vm, mrb_value *v, int argc) {
    auto client = *(HTTPClient**) v[0].instance->data;
    auto r = client->getString();
    SET_RETURN(mrbc_string_new_cstr(vm, r.c_str()));
    return;
}

static void c_httpclient_size(mrb_vm *vm, mrb_value *v, int argc) {
    auto client = *(HTTPClient**) v[0].instance->data;
    auto r = client->getSize();
    SET_INT_RETURN(r);
    return;
}
static void c_httpclient_post(mrb_vm *vm, mrb_value *v, int argc) {
    auto client = *(HTTPClient**) v[0].instance->data;
    if(argc < 1){
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "need argument of payload");
        return;
    }

    if(v[1].tt == MRBC_TT_STRING){   // do not to use val_to_s for included '\0'
        uint8_t* payload  = (uint8_t*) v[1].string->data;
        uint16_t len = v[1].string->size;
        auto r = client->POST(payload,len);
        SET_INT_RETURN(r);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "String is expected for payload");
        SET_NIL_RETURN();
    }
    return;
}

static void c_httpclient_put(mrb_vm *vm, mrb_value *v, int argc) {
    auto client = *(HTTPClient**) v[0].instance->data;
    if(argc < 1){
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "need argument of payload");
        return;
    }

    if(v[1].tt == MRBC_TT_STRING){   // do not to use val_to_s for included '\0'
        uint8_t* payload  = (uint8_t*) v[1].string->data;
        uint16_t len = v[1].string->size;
        auto r = client->PUT(payload,len);
        SET_INT_RETURN(r);
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "String is expected for payload");
        SET_NIL_RETURN();
    }
    return;
}

static void c_httpclient_end(mrb_vm *vm, mrb_value *v, int argc) {
    auto client = *(HTTPClient**) v[0].instance->data;
    client->end();
    return;
}

#endif // USE_HTTPCLIENT

void class_wifi_init(){
    mrb_class *class_wifi = mrbc_define_class(0, "Wifi", mrbc_class_object);
    mrbc_define_method(0, class_wifi, "begin", c_wifi_begin);
    mrbc_define_method(0, class_wifi, "connect", c_wifi_begin);
    mrbc_define_method(0, class_wifi, "status", c_wifi_status);
    mrbc_define_method(0, class_wifi, "wait_for_connect", c_wifi_wait_for_connect);
    mrbc_define_method(0, class_wifi, "mode", c_wifi_mode);
    mrbc_define_method(0, class_wifi, "set_mode", c_wifi_set_mode);
    mrbc_define_method(0, class_wifi, "mode=", c_wifi_set_mode);
    mrbc_define_method(0, class_wifi, "disconnect", c_wifi_disconnect);
    mrbc_define_method(0, class_wifi, "softap", c_wifi_softap);
#ifdef USE_HTTPCLIENT
    class_httpclient = mrbc_define_class(0, "HTTPClient", mrbc_class_object);
    mrbc_define_method(0, class_httpclient, "new", c_httpclient_new);
    mrbc_define_method(0, class_httpclient, "initialize", c_httpclient_initialize);
    mrbc_define_method(0, class_httpclient, "get", c_httpclient_get);
    mrbc_define_method(0, class_httpclient, "response", c_httpclient_get_response);
    mrbc_define_method(0, class_httpclient, "size", c_httpclient_size);
    mrbc_define_method(0, class_httpclient, "post", c_httpclient_post);
    mrbc_define_method(0, class_httpclient, "put", c_httpclient_put);
    mrbc_define_method(0, class_httpclient, "close", c_httpclient_end);
    mrbc_define_method(0, class_httpclient, "end", c_httpclient_end);

#endif // USE_HTTPCLIENT   

}