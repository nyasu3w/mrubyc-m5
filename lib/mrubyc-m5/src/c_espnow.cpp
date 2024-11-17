#include "my_mrubydef.h"
#include "mrubyc.h"

#ifdef USE_ESPNOW
#include <WiFi.h>
#include <esp_now.h>

#include "c_espnow.h"

constexpr uint8_t PEER_MAX = 4;
static uint8_t paired_num=0;
static esp_now_peer_info_t peer_info[PEER_MAX];

// Ring buffer
typedef enum { RECV_POLICY_RING, RECV_POLICY_REJECT_BY_FULL} policy_t;
static policy_t recv_policy = RECV_POLICY_RING;
constexpr uint8_t RECV_MAX = 8;
static struct {
    uint8_t peer_addr[6];
//    uint8_t* data;
//    size_t size;
    mrbc_value string;
} m_espnow_recvdata[RECV_MAX];
static uint8_t recv_data_in = 0;
static uint8_t recv_data_out = 0;

static esp_now_send_status_t last_send_status = ESP_NOW_SEND_SUCCESS;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    last_send_status = status;
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {  // need sync with class_espnow_recv() for ring buffer
    uint8_t recv_data_in_next = (recv_data_in+1)%RECV_MAX;
    if(recv_policy==RECV_POLICY_REJECT_BY_FULL && recv_data_in_next==recv_data_out){
        return; // full
    }

    for(int i=0;i<6;i++){
        m_espnow_recvdata[recv_data_in].peer_addr[i] = mac_addr[i];
    }
    m_espnow_recvdata[recv_data_in].string = mrbc_string_new(0, data, data_len);
    recv_data_in = recv_data_in_next;
}


static void
class_espnow_begin(mrb_vm *vm, mrb_value *v, int argc) {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();   // esp_now_ini() cause reset without these two lines

    if(esp_now_init() != ESP_OK){
        SET_FALSE_RETURN();
        return;
    } 
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);
    SET_TRUE_RETURN();
}

static uint8_t find_free_peernumber(){
    if(paired_num<PEER_MAX-1){
        return paired_num;
    };
    for(int i=0;i<PEER_MAX;i++){
        if(esp_now_is_peer_exist(peer_info[i].peer_addr)==false){
            return i;
        }
    }
    return 0xff;
}

static bool
add_espnow_peer(uint8_t no, mrb_value *ary, mrb_vm *vm, mrb_value *v, int argc){

// ary is  n array of [channel, [MAC addr bytes]]
    if(!mrbc_obj_is_kind_of(ary,MRBC_CLASS(Array))){
        return false;  // need that ary is an array.
    }
    if(ary->array->n_stored<2) {
        return false;  // need 6 items
    }
    mrbc_value temp_val = mrbc_array_get(ary,0);
    if(!mrbc_obj_is_kind_of(&temp_val,MRBC_CLASS(Integer))){
        return false;  // need a channel number
    }
    uint8_t chan = val_to_i(vm,v,temp_val,argc);
    temp_val = mrbc_array_get(ary,1);
    if(!mrbc_obj_is_kind_of(&temp_val,MRBC_CLASS(Array))){
        return false;  // need an array of MAC addr
    }
    if(chan>13){
        return false;  // channel number is 0-13
    }

    temp_val = mrbc_array_get(ary,1);
    if(temp_val.array->n_stored<6){
        return false;  // need 6 items
    }
    // format check done

    uint8_t peer_no = find_free_peernumber();
    if(peer_no>=PEER_MAX){
        return false;  // no free peer number
    }
    peer_info[peer_no].channel = chan;
    for(int i=0;i<6;i++){   // 6 is a size of MAC addr
        mrbc_value octed_val = mrbc_array_get(&temp_val,i);
        peer_info[peer_no].peer_addr[i] = val_to_i(vm,v,octed_val,argc);
    }
    if(esp_now_add_peer(&peer_info[peer_no])==ESP_OK) {
        return true;
    }
    return false;
}

static void
class_espnow_pair(mrb_vm *vm, mrb_value *v, int argc){

    if(paired_num>=PEER_MAX){
        SET_FALSE_RETURN();
        return; // already full
    }
   
    if(add_espnow_peer(paired_num,&GET_ARG(1), vm, v, argc)){
        SET_INT_RETURN(paired_num++);
    } else {
        SET_FALSE_RETURN();
    }

}

static void
class_espnow_del_peer(mrb_vm *vm, mrb_value *v, int argc){
    if(argc!=1){
        SET_FALSE_RETURN();
        return;
    }
    uint8_t peer = val_to_i(vm,v,v[1],1);
    if(peer>=paired_num){
        SET_FALSE_RETURN();
        return;
    }
    if(esp_now_del_peer(peer_info[peer].peer_addr)==ESP_OK){
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}


static void
class_espnow_send(mrb_vm *vm, mrb_value *v, int argc){  // written by copilot
    if(argc!=2){
        SET_FALSE_RETURN();
        return;
    }
    if(!mrbc_obj_is_kind_of(&v[1],MRBC_CLASS(Integer))){
        SET_FALSE_RETURN();
        return;
    }
    if(!mrbc_obj_is_kind_of(&v[2],MRBC_CLASS(String))){
        SET_FALSE_RETURN();
        return;
    }
    int peer = val_to_i(vm,v,v[1],1);
    if(peer>=paired_num){
        SET_FALSE_RETURN();
        return;
    }
    esp_err_t err = esp_now_send(peer_info[peer].peer_addr,(uint8_t*)v[2].string->data,v[2].string->size);
    if(err==ESP_OK){
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void
class_espnow_recv(mrb_vm *vm, mrb_value *v, int argc){
    if(recv_data_out==recv_data_in){
        SET_NIL_RETURN();
    } else if(argc>0){
        int rtype=val_to_i(vm,v,v[1],argc);
        switch(rtype) {
            case 0: // return only data
                SET_RETURN(m_espnow_recvdata[recv_data_out].string);
                break;
            case 1: {// return array of [data, peer]
                    mrbc_value recv_data =mrbc_array_new(vm,2);
                    mrbc_array_set(&recv_data,0,&m_espnow_recvdata[recv_data_out].string);
                    mrbc_value recv_data_peer = mrbc_array_new(vm,6);
                    mrbc_array_set(&recv_data,1,&recv_data_peer);
                    for(int i=0;i<6;i++){
                        mrbc_value octed=mrbc_fixnum_value(m_espnow_recvdata[recv_data_out].peer_addr[i]);
                        mrbc_array_set(&recv_data_peer,i,&octed);
                    }
                    SET_RETURN(recv_data);
                }
                break; 
            default:
                SET_FALSE_RETURN();
                break;
        }
    } else { // argc>0
        SET_RETURN(m_espnow_recvdata[recv_data_out].string);
    }
    recv_data_out = (recv_data_out+1)%RECV_MAX;
}

static void
class_espnow_recv_count(mrb_vm *vm, mrb_value *v, int argc){
    int8_t count = recv_data_in-recv_data_out;
    if(count<0){
        count += RECV_MAX;
    }
    SET_INT_RETURN(count);
}

static void
class_espnow_get_policy(mrb_vm *vm, mrb_value *v, int argc){
    if(recv_policy==RECV_POLICY_RING){
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void
class_espnow_set_policy(mrb_vm *vm, mrb_value *v, int argc){
    if(argc!=1){
        SET_FALSE_RETURN();
        return;
    }
    if(mrbc_obj_is_kind_of(&v[1],MRBC_CLASS(TrueClass))) {
        recv_policy = RECV_POLICY_RING;
    } else {
        recv_policy = RECV_POLICY_REJECT_BY_FULL;
    }
}

static void
class_espnow_last_send_status(mrb_vm *vm, mrb_value *v, int argc){ // written by copilot
    switch(last_send_status){
        case ESP_NOW_SEND_SUCCESS:
            SET_TRUE_RETURN();
            break;
        case ESP_NOW_SEND_FAIL:
            SET_FALSE_RETURN();
            break;
        default:
            SET_NIL_RETURN();
            break;
    }
}

void class_espnow_init() {
    mrb_class *class_espnow = mrbc_define_class(0,"EspNow",mrbc_class_object);
    mrbc_define_method(0, class_espnow, "begin", class_espnow_begin);
    mrbc_define_method(0, class_espnow, "add_peer", class_espnow_pair);
    mrbc_define_method(0, class_espnow, "del_peer", class_espnow_del_peer);
    mrbc_define_method(0, class_espnow, "send", class_espnow_send);
    mrbc_define_method(0, class_espnow, "recv", class_espnow_recv);
    mrbc_define_method(0, class_espnow, "received", class_espnow_recv_count);
    mrbc_define_method(0, class_espnow, "get_policy", class_espnow_get_policy);
    mrbc_define_method(0, class_espnow, "policy", class_espnow_get_policy);
    mrbc_define_method(0, class_espnow, "set_policy", class_espnow_set_policy);
    mrbc_define_method(0, class_espnow, "policy=", class_espnow_set_policy);
    mrbc_define_method(0, class_espnow, "last_send_status", class_espnow_last_send_status);
}

#endif // USE_ESPNOW

