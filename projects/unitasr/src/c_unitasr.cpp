#include <map>
#include <M5Unified.h>
#include <HardwareSerial.h>

#include "my_mrubydef.h"
#include "c_unitasr.h"

HardwareSerial UnitASR(1); // Use Serial1 for UnitASR

static void c_unitasr_receive_command(mrb_vm *vm, mrb_value *v, int argc) {
    static uint8_t received = 0;
    static uint8_t third_byte = 0;
    unsigned long start_time = millis();
    unsigned long timeout;

    if(argc > 0) {
        timeout = val_to_i(vm, v, GET_ARG(1), argc);
    } else {
        timeout = 0; // Default timeout
    }

    // check byte sequence  as 0xaa 0x55 xx 0x55 0xaa ( xx is any byte)
    while (received < 5) {
        if(UnitASR.available()) {
            uint8_t byte = UnitASR.read();
            if (received == 0 && byte == 0xaa) {
                received++;
            } else if (received == 1 && byte == 0x55) {
                received++;
            } else if (received == 2) {
                // Store the third byte
                third_byte = byte;
                received++;
            } else if (received == 3 && byte == 0x55) {
                received++;
            } else if (received == 4 && byte == 0xaa) {
                // Command complete
                received++;
                break;
            } else {
                // Reset if the sequence is broken
                received = 0;
            }
        } else {
            // No data available, wait for a short period
            delay(50);
        }
        if(timeout > 0 && millis() >start_time + timeout) {
            // Timeout
            break;
        }
    }

    if(received==5) {
        received = 0;
        SET_INT_RETURN(third_byte);
    } else if(received>0) {
        SET_FALSE_RETURN();
    } else {
        SET_NIL_RETURN();
    }
}

static void c_unitasr_default_vocabulary(mrb_vm *vm, mrb_value *v, int argc) {
    static std::map<uint8_t, const char*> commandList = {
        {0x00, "Unknown command word"},
        {0x01, "up"},
        {0x02, "down"},
        {0x03, "left"},
        {0x04, "turn left"},
        {0x05, "right"},
        {0x06, "turn right"},
        {0x07, "forward"},
        {0x08, "front"},
        {0x09, "backward"},
        {0x0A, "back"},
        {0x10, "open"},
        {0x11, "close"},
        {0x12, "start"},
        {0x13, "stop"},
        {0x14, "turn on"},
        {0x15, "turn off"},
        {0x16, "play"},
        {0x17, "pause"},
        {0x18, "turn on the lights"},
        {0x19, "turn off the lights"},
        {0x1A, "previous"},
        {0x1B, "next"},
        {0x20, "zero"},
        {0x21, "one"},
        {0x22, "two"},
        {0x23, "three"},
        {0x24, "four"},
        {0x25, "five"},
        {0x26, "six"},
        {0x27, "seven"},
        {0x28, "eight"},
        {0x29, "nine"},
        {0x30, "ok"},
        {0x31, "hi, A S R"},
        {0x32, "hello"},
        {0x40, "increase volume"},
        {0x41, "decrease volume"},
        {0x42, "maximum volume"},
        {0x43, "medium volume"},
        {0x44, "minimum volume"},
        {0x45, "check firmware version"},
        {0xFE, "Announce"},
        {0xFF, "Hi,M Five"},
    };

    if(argc > 0) {
        int vocab = val_to_i(vm, v, GET_ARG(1), argc);
        SET_RETURN(mrbc_string_new_cstr(vm, commandList[vocab]));
    } else {
        mrbc_raise(vm, MRBC_CLASS(ArgumentError), "too few arguments");
        SET_FALSE_RETURN();
    }
}




static void initialize_unitasr() {
    // Initialize the UnitASR module
    gpio_num_t rx_pin = (gpio_num_t)M5.getPin(m5::port_a_pin1);
    gpio_num_t tx_pin = (gpio_num_t)M5.getPin(m5::port_a_pin2);
    printf("getPin: RX:%d TX:%d\n", rx_pin, tx_pin);
    UnitASR.begin(115200, SERIAL_8N1, rx_pin, tx_pin);
}

void class_unitasr_init() {
    initialize_unitasr();
    mrb_class *class_unitasr = mrbc_define_class(0, "UnitASR", mrbc_class_object);
    mrbc_define_method(0, class_unitasr, "receive", c_unitasr_receive_command);
    mrbc_define_method(0, class_unitasr, "command_str", c_unitasr_default_vocabulary);
}

