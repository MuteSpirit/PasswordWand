#pragma once

#ifndef __BOARD_HPP__
#define __BOARD_HPP__

// Using Uno only for develop part of features.
// Arduino Pro Micro is target device.
#define BOARD_ARDUINO_UNO

#include <GyverOLED.h>
#include <SPI.h>
#include <EEPROM_SPI_WE.h>

#ifdef BOARD_ARDUINO_UNO

#define BTN_SQUARE_PIN    2
#define BTN_TRIANGLE_PIN  4
#define BTN_CIRCLE_PIN    7
#define BTN_CROSS_PIN     8

#define csPin PD5
#define wpPin 999 /* fake. On board it's pulled up to 5V */

#define ROTARY_DT_PIN  A2
#define ROTARY_CLK_PIN A3

#else // for AtMega32u4 / Arduino Pro Mini

#define BTN_SQUARE_PIN    2
#define BTN_TRIANGLE_PIN  4
#define BTN_CIRCLE_PIN    7
#define BTN_CROSS_PIN     8

#define ROTARY_CLK_PIN 9
#define ROTARY_DT_PIN  8
                                                                                //
#endif // BOARD_ARDUINO_UNO



extern EEPROM_SPI_WE eep;


/// There are 4 buttons on the device with geometric figures on them:
enum DeviceButton
{
   button_square,
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
typedef GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> Display;
extern Display oled;

#endif // !__BOARD_HPP__
