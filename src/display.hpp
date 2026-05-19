#pragma once

#ifndef __DISPLAY_HPP__
#define __DISPLAY_HPP__

#include <Print.h>
#include <GyverOLED.h>

struct Account;

Print& display_init();

typedef GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> Display;

#endif // __DISPLAY_HPP__
