#pragma once

#ifndef __BOARD_HPP__
#define __BOARD_HPP__

// GyverOLED does not allow to set font
// GyverOLEDMenu draw ugly menu items
// Both will be replaced
// #define USE_MICRO_WIRE // for decrease size of code
// #include "GyverOLED/GyverOLED.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#include <SPI.h>
#include <EEPROM_SPI_WE.h>

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



extern EEPROM_SPI_WE eep;


/// There are 4 buttons on the device with geometric figures on them:
enum DeviceButton
{
   button_square = 0,
   button_triangle,
   button_circle,
   button_cross,
   num_of_buttons
};

typedef void (*ButtonCallback)(void*);

void set_button_callback(DeviceButton btn, ButtonCallback cb, void *ctx);
void stub_btn_cb(void *);

typedef void (*RotateCallback)(void *ctx, int direction);
void set_rotate_callback(RotateCallback cb, void *ctx);
void stub_rotate_cb(void *, int);

void board_setup(void);
void board_loop_step(void);


// Use OLED_NO_BUFFER to keep RAM for another code
// typedef GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> Display;
typedef SSD1306AsciiAvrI2c Display;
extern Display oled;

#endif // !__BOARD_HPP__
