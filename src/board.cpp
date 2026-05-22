#include <inttypes.h>
#include <Arduino.h>
#include <RotaryEncoder.h>

#include "board.hpp"

EEPROM_SPI_WE eep(EXT_EEPROM_CS_PIN, EXT_EEPROM_WP_PIN, 1000000);

RotaryEncoder rotaryEncoder(ROTARY_CLK_PIN, ROTARY_DT_PIN);

struct ButtonHook
{
    ButtonCallback cb;
    void *ctx;

    bool push_happen_;
};

ButtonHook btn_hooks[DeviceButton::num_of_buttons];

struct RotateHook
{
    RotateCallback cb;
    void *ctx;
};

RotateHook encoder_hook;

uint8_t
btn2pin(DeviceButton btn)
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

const char*
btn2name(DeviceButton btn)
{
    switch (btn) {
        case DeviceButton::button_square: return "SQUARE";
        case DeviceButton::button_triangle: return "TRIANGLE";
        case DeviceButton::button_circle: return "CIRCLE";
        case DeviceButton::button_cross: return "CROSS";
        default:
           return "UNKNOWN";
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
    for (uint8_t i = DeviceButton::button_square; i < DeviceButton::num_of_buttons; ++i) {
        btn_hooks[i].cb = NULL;
        btn_hooks[i].ctx = NULL;
        btn_hooks[i].push_happen_ = false;
    }

    // pinMode(BTN_SQUARE_PIN, INPUT_PULLUP);
    // pinMode(BTN_TRIANGLE_PIN, INPUT_PULLUP);
    // pinMode(BTN_CIRCLE_PIN, INPUT_PULLUP);
    // pinMode(BTN_CROSS_PIN, INPUT_PULLUP);

    // Use external pullup resistors
    pinMode(BTN_SQUARE_PIN, INPUT);
    pinMode(BTN_TRIANGLE_PIN, INPUT);
    pinMode(BTN_CIRCLE_PIN, INPUT);
    pinMode(BTN_CROSS_PIN, INPUT);

    // EEPROM_SPI_WE lib sets CS EEPROM pin to OUTPU itself
    // pinMode(EXT_EEPROM_CS_PIN, OUTPUT);
    // digitalWrite(EXT_EEPROM_CS_PIN, HIGH); // Start with EEPROM not selected

    // RotaryEncoder lib configures encoder pins itself
    // pinMode(ROTARY_CLK_PIN, INPUT_PULLUP);
    // pinMode(ROTARY_DT_PIN, INPUT_PULLUP);
}

static void
check_button(DeviceButton btn)
{
    // Physycal debounce using RC-filter did not help with simplest buttons I have
    // Let's try to react only on changing to sequence of signals HIGH (by default) then LOW then HIGH again

    if (NULL == btn_hooks[btn].cb) {
        return;
    }

    const uint8_t i = btn2pin(btn);
    if (btn_hooks[i].push_happen_) {
        if (HIGH == digitalRead(i)) {
            Serial.print(btn2name(btn));
            Serial.println(F(": release"));

            btn_hooks[i].push_happen_ = false;
            (*btn_hooks[btn].cb)(btn_hooks[btn].ctx);
        }
    } else {
        if (LOW == digitalRead(i)) {
            Serial.print(btn2name(btn));
            Serial.println(F(": push"));

            btn_hooks[i].push_happen_ = true;
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
