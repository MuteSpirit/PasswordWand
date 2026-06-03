#pragma once
#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <RotaryEncoder.h>
#include "user_inputs.hpp"



// Pins definition:
//
// #if defined(ARDUINO_AVR_UNO)
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
// #elif defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MICRO)
//
// 25LC256 EEPROM chip
#define EXT_EEPROM_CS_PIN    10    // 1. Chip Select Pin.
#define EXT_EEPROM_CO_PIN    MISO  // 2. Slave Out. To MISO on board. On Pro Micro - 14
#define EXT_EEPROM_WP_PIN    999   // 3. Write Protect Pin. fake value. On board it's pulled up to 5V
#define EXT_EEPROM_GND_PIN   GND   // 4. Ground
#define EXT_EEPROM_SI_PIN    MOSI  // 5. Slave Input. To MOSI on board. On Pro Mini - 16
#define EXT_EEPROM_SCK_PIN   SCK   // 6. SPI Clock. To SCK on board, On Pro Mini - 15
#define EXT_EEPROM_HOLD_PIN  VCC   // 7. Hold pin, active-low, pauses the data transfer.
#define EXT_EEPROM_VCC_PIN   VCC   // 8. Power 

// #else
// #error("Unknown board type")
// #endif // ARDUINO_AVR_UNO

// OLED Display
#define OLED_SCK_PIN    3
#define OLED_SDA_PIN    2
#define OLED_I2C_ADDR   0x3C


////////////////////////////////////////////////////////////////////////////////
class DeviceUserInputs : public UserInputs
{
public:
    DeviceUserInputs();

    void setup(); /// call from sketch setup()
    void loop_step(); /// call from sketch loop()

    virtual void set(Button btn, BlindCall cb) override;
    virtual void unset(Button btn) override;

    virtual void set(Encoder, BlindCall cb) override;
    virtual void unset(Encoder) override;

protected:
    void checkButton(UserInputs::Button btn);
    void checkEncoder(UserInputs::Encoder enc);

protected:
    struct ButtonHook
    {
        BlindCall cb_     {BlindCall::stub()};
        bool push_happen_ {false};
    };

    ButtonHook btnHooks_[static_cast<uint8_t>(UserInputs::Button::num_of_buttons)];

    BlindCall encoderHooks_[static_cast<uint8_t>(UserInputs::Encoder::num_of_encoders)] {BlindCall::stub()};

    RotaryEncoder rotaryEncoder_;
};

#endif // !__BOARD_HPP__
