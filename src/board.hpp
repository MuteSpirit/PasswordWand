#pragma once

#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <GyverOLED.h>

#ifdef BOARD_ARDUINO_UNO

#define BTN_SQUARE_PIN    2
#define BTN_TRIANGLE_PIN  4
#define BTN_CIRCLE_PIN    7
#define BTN_CROSS_PIN     8

#else

#define BTN_SQUARE_PIN    2
#define BTN_TRIANGLE_PIN  4
#define BTN_CIRCLE_PIN    7
#define BTN_CROSS_PIN     8

#endif // BOARD_ARDUINO_UNO

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
