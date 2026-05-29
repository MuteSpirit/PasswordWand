#include <inttypes.h>
#include <Arduino.h>

#include "board.hpp"

// Pins definition:
//
// #if defined(ARDUINO_AVR_UNO)
//
// // Buttons
// #define BTN_SQUARE_PIN    2
// #define BTN_TRIANGLE_PIN  4
// #define BTN_CIRCLE_PIN    7
// #define BTN_CROSS_PIN     8
//
// // 25LC256 EEPROM chip
// #define EXT_EEPROM_CS_PIN    5     // 1. Chip Select Pin. On Uno - PD5 / D5 / 5
// #define EXT_EEPROM_CO_PIN    12    // 2. Slave Out. To MISO on board
// #define EXT_EEPROM_WP_PIN    999   // 3. Write Protect Pin. fake value. On board it's pulled up to 5V
// #define EXT_EEPROM_GND_PIN   GND   // 4. Ground
// #define EXT_EEPROM_SI_PIN    MOSI  // 5. Slave Input. To MOSI on board, On Uno - 11 / MOSI
// #define EXT_EEPROM_SCK_PIN   SCK   // 6. SPI Clock. To SCK on board. On Uno - 13
// #define EXT_EEPROM_HOLD_PIN  HOLD  // 7. Hold pin, active-low, pauses the data transfer.
// #define EXT_EEPROM_VCC_PIN   VCC   // 8. Power 
//
// // Rotary Encoder
// #define ROTARY_CLK_PIN A2
// #define ROTARY_DT_PIN  A3
//
// // OLED Display
// #define OLED_SCK_PIN    A5
// #define OLED_SDA_PIN    A4
// #define OLED_I2C_ADDR   0x3C
//
// #elif defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MICRO)

// Buttons
#define BTN_SQUARE_PIN    1  // Pin 4 should be here but it is broken on alone board I have ((
#define BTN_TRIANGLE_PIN  2  // Pin 5 should be here but it bacome unstable on my board (((
#define BTN_CIRCLE_PIN    6
#define BTN_CROSS_PIN     7

// 25LC256 EEPROM chip
#define EXT_EEPROM_CS_PIN    10    // 1. Chip Select Pin.
#define EXT_EEPROM_CO_PIN    MISO  // 2. Slave Out. To MISO on board. On Pro Micro - 14
#define EXT_EEPROM_WP_PIN    999   // 3. Write Protect Pin. fake value. On board it's pulled up to 5V
#define EXT_EEPROM_GND_PIN   GND   // 4. Ground
#define EXT_EEPROM_SI_PIN    MOSI  // 5. Slave Input. To MOSI on board. On Pro Mini - 16
#define EXT_EEPROM_SCK_PIN   SCK   // 6. SPI Clock. To SCK on board, On Pro Mini - 15
#define EXT_EEPROM_HOLD_PIN  VCC   // 7. Hold pin, active-low, pauses the data transfer.
#define EXT_EEPROM_VCC_PIN   VCC   // 8. Power 

// Rotary Encoder
#define ROTARY_CLK_PIN   8
#define ROTARY_DT_PIN    9

// OLED Display
#define OLED_SCK_PIN    3
#define OLED_SDA_PIN    2
#define OLED_I2C_ADDR   0x3C

// #else
// #error("Unknown board type")
// #endif // ARDUINO_AVR_UNO

// Will be used via EepromStorage:
// EEPROM_SPI_WE eep(EXT_EEPROM_CS_PIN, EXT_EEPROM_WP_PIN, 1000000);

////////////////////////////////////////////////////////////////////////////////
DeviceUserInputs::DeviceUserInputs()
    : rotaryEncoder_(ROTARY_CLK_PIN, ROTARY_DT_PIN)
{}

void
DeviceUserInputs::set(Button btn, BlindCall cb)
{
    btnHooks_[static_cast<uint8_t>(btn)].cb_ = cb;
}

void
DeviceUserInputs::unset(Button btn)
{
    btnHooks_[static_cast<uint8_t>(btn)].cb_ = BlindCall::stub();
}

void
DeviceUserInputs::set(Encoder enc, BlindCall cb)
{
    encoderHooks_[static_cast<uint8_t>(enc)] = cb;
}

void
DeviceUserInputs::unset(Encoder enc)
{
    encoderHooks_[static_cast<uint8_t>(enc)] = BlindCall::stub();
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
    if (!btnHooks_[static_cast<uint8_t>(btn)].cb_) {
        return;
    }

    uint8_t pin = btn2pin(btn);

    if (btnHooks_[static_cast<uint8_t>(btn)].push_happen_) {
        if (HIGH == digitalRead(pin)) {
            btnHooks_[static_cast<uint8_t>(btn)].push_happen_ = false;
            btnHooks_[static_cast<uint8_t>(btn)].cb_();
        }
    } else {
        if (LOW == digitalRead(pin)) {
            btnHooks_[static_cast<uint8_t>(btn)].push_happen_ = true;
        }
    }
}

void
DeviceUserInputs::checkEncoder(UserInputs::Encoder enc)
{
    if (!encoderHooks_[static_cast<uint8_t>(enc)]) {
        return;
    }

    rotaryEncoder.tick();

    int dir = (int)rotaryEncoder.getDirection();
    encoderHooks_[static_cast<uint8_t>(enc)](dir);
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
