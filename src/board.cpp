#include <inttypes.h>
#include <Arduino.h>
#include <RotaryEncoder.h>

#include "board.hpp"
#include "blind_call.hpp"
#include "user_inputs.hpp"

EEPROM_SPI_WE eep(EXT_EEPROM_CS_PIN, EXT_EEPROM_WP_PIN, 1000000);

RotaryEncoder rotaryEncoder(ROTARY_CLK_PIN, ROTARY_DT_PIN);


////////////////////////////////////////////////////////////////////////////////
void
DeviceUserInputs::set(Button btn, BlindCall cb)
{
    btn_hooks_[static_cast<uint8_t>(btn)].cb_ = cb;
}

void
DeviceUserInputs::unset(Button btn)
{
    btn_hooks_[static_cast<uint8_t>(btn)].cb_ = BlindCall::stub();
}

void
DeviceUserInputs::set(Encoder enc, BlindCall cb)
{
    encoder_hooks_[static_cast<uint8_t>(enc)] = cb;
}

void
DeviceUserInputs::unset(Encoder enc)
{
    encoder_hooks_[static_cast<uint8_t>(enc)] = BlindCall::stub();
}

static uint8_t btn2pin(UserInputs::Button btn);

void
DeviceUserInputs::setup(void)
{
    pinMode(BTN_SQUARE_PIN, INPUT_PULLUP);
    pinMode(BTN_TRIANGLE_PIN, INPUT_PULLUP);
    pinMode(BTN_CIRCLE_PIN, INPUT_PULLUP);
    pinMode(BTN_CROSS_PIN, INPUT_PULLUP);

    // Use external pullup resistors
    // pinMode(BTN_SQUARE_PIN, INPUT);
    // pinMode(BTN_TRIANGLE_PIN, INPUT);
    // pinMode(BTN_CIRCLE_PIN, INPUT);
    // pinMode(BTN_CROSS_PIN, INPUT);
    //
    // EEPROM_SPI_WE lib sets CS EEPROM pin to OUTPU itself
    // pinMode(EXT_EEPROM_CS_PIN, OUTPUT);
    // digitalWrite(EXT_EEPROM_CS_PIN, HIGH); // Start with EEPROM not selected

    // RotaryEncoder lib configures encoder pins itself
    // pinMode(ROTARY_CLK_PIN, INPUT_PULLUP);
    // pinMode(ROTARY_DT_PIN, INPUT_PULLUP);
}

void
DeviceUserInputs::checkButton(UserInputs::Button btn)
{
    // Physycal debounce using RC-filter did not help with simplest buttons I have
    // Let's try to react only on changing to sequence of signals HIGH (by default) then LOW then HIGH again
    if (!btn_hooks_[static_cast<uint8_t>(btn)].cb_) {
        return;
    }

    uint8_t pin = btn2pin(btn);

    if (btn_hooks_[static_cast<uint8_t>(btn)].push_happen_) {
        if (HIGH == digitalRead(pin)) {
            btn_hooks_[static_cast<uint8_t>(btn)].push_happen_ = false;
            btn_hooks_[static_cast<uint8_t>(btn)].cb_();
        }
    } else {
        if (LOW == digitalRead(pin)) {
            btn_hooks_[static_cast<uint8_t>(btn)].push_happen_ = true;
        }
    }
}

void
DeviceUserInputs::checkEncoder(UserInputs::Encoder enc)
{
    if (!encoder_hooks_[static_cast<uint8_t>(enc)]) {
        return;
    }

    rotaryEncoder.tick();

    int dir = (int)rotaryEncoder.getDirection();
    encoder_hooks_[static_cast<uint8_t>(enc)](dir);
}

void
DeviceUserInputs::loop_step(void)
{
    checkEncoder(UserInputs::Encoder::rotary);

    checkButton(UserInputs::Button::square);
    checkButton(UserInputs::Button::triangle);
    checkButton(UserInputs::Button::circle);
    checkButton(UserInputs::Button::cross);
}

static uint8_t
btn2pin(UserInputs::Button btn)
{
    switch (btn) {
        case UserInputs::Button::square: return BTN_SQUARE_PIN;
        case UserInputs::Button::triangle: return BTN_TRIANGLE_PIN;
        case UserInputs::Button::circle: return BTN_CIRCLE_PIN;
        case UserInputs::Button::cross: return BTN_CROSS_PIN;
        default:
           return 0;
    }
}

/// @brief For debug. Uncomment on demand.
// static const char*
// btn2name(UserInputs::Button btn)
// {
//     switch (btn) {
//         case UserInputs::Button::square: return "SQUARE";
//         case UserInputs::Button::triangle: return "TRIANGLE";
//         case UserInputs::Button::circle: return "CIRCLE";
//         case UserInputs::Button::cross: return "CROSS";
//         default:
//            return "UNKNOWN";
//     }
// }
