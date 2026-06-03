#include "ssd1306_oled.hpp"

// GyverOLED does not allow to set font
// GyverOLEDMenu draw ugly menu items
// Both will be replaced
// #define USE_MICRO_WIRE // for decrease size of code
// #include "GyverOLED/GyverOLED.h"

// Let's use OLED library as in original PasswordPump project
#include "SSD1306AsciiAvrI2c.h"

#include "board.hpp"

// Use OLED_NO_BUFFER to keep RAM for another code
// typedef GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> Display;
typedef SSD1306AsciiAvrI2c Display;
Display oled_;


void
SSD1306oled::clear()
{
    oled_.clear();
}

void
SSD1306oled::home()
{
    oled_.home();
}

void
SSD1306oled::setup()
{
  oled_.begin(&Adafruit128x32, OLED_I2C_ADDR);
  // oled_.init(OLED_SDA_PIN, OLED_SCK_PIN);
}

void
SSD1306oled::setFont(const uint8_t* font)
{
    oled_.setFont(font);                                                // perfect, slightly smaller than Arial14
}

size_t
SSD1306oled::write(uint8_t b)
{
    return oled_.write(b);
}

size_t
SSD1306oled::write(const uint8_t *buffer, size_t size)
{
    return oled_.write(buffer, size);
}
