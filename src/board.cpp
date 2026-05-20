#include <inttypes.h>
#include <Arduino.h>
#include <RotaryEncoder.h>

#include "board.hpp"

// Use OLED_NO_BUFFER to keep RAM for another code
Display oled;

EEPROM_SPI_WE eep(csPin, wpPin, 1000000);

RotaryEncoder rotaryEncoder(ROTARY_DT_PIN, ROTARY_CLK_PIN/*, RotaryEncoder::LatchMode::TWO03*/);

struct ButtonHook
{
    ButtonCallback cb;
    void *ctx;
};

ButtonHook btn_hooks[DeviceButton::num_of_buttons];

struct RotateHook
{
    RotateCallback cb;
    void *ctx;
};

RotateHook encoder_hook;

uint8_t btn2pin(DeviceButton btn)
{
    switch (btn) {
        case DeviceButton::button_square: return BTN_SQUARE_PIN;
        case DeviceButton::button_triangle: return BTN_TRIANGLE_PIN;
        case DeviceButton::button_circle: return BTN_CIRCLE_PIN;
        case DeviceButton::button_cross: return BTN_CROSS_PIN;
        default:
           return DeviceButton::num_of_buttons;
    }
}

void
set_button_callback(DeviceButton btn, void (*cb)(void*), void *ctx)
{
    btn_hooks[btn].cb = cb;
    btn_hooks[btn].ctx = ctx;
}

void
set_rotate_callback(RotateCallback cb, void *ctx)
{
    encoder_hook.cb = cb;
    encoder_hook.ctx = ctx;
}

void
board_setup(void)
{
    pinMode(BTN_SQUARE_PIN, INPUT_PULLUP);
    pinMode(BTN_TRIANGLE_PIN, INPUT_PULLUP);
    pinMode(BTN_CIRCLE_PIN, INPUT_PULLUP);
    pinMode(BTN_CROSS_PIN, INPUT_PULLUP);
    // RotaryEncoder lib configures pins itself
    // pinMode(ROTARY_CLK_PIN, INPUT_PULLUP);
    // pinMode(ROTARY_DT_PIN, INPUT_PULLUP);
}

static void
check_button(DeviceButton btn)
{
    if (LOW == digitalRead(btn2pin(btn))) {
        if (btn_hooks[btn].cb) {
            (*btn_hooks[btn].cb)(btn_hooks[btn].ctx);
        }
    } 
}

static void
check_encoder(void)
{
    if (encoder_hook.cb) {
        rotaryEncoder.tick();

        int dir = (int)rotaryEncoder.getDirection();
        if (0 != dir) {
            (*encoder_hook.cb)(encoder_hook.ctx, dir);
        }
    }

}

void
board_loop_step(void)
{
    check_encoder();
    check_button(button_square);
    check_button(button_triangle);
    check_button(button_circle);
    check_button(button_cross);
}

void
stub_btn_cb(void *)
{}

void
stub_rotate_cb(void *, int)
{}
