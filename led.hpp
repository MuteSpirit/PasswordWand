#pragma once

#ifndef __LED_HPP__
#define __LED_HPP__

#include <inttypes.h>

void setPurple(void);
void setRed(void);
void setGreen(void);
void setYellow(void);
void setBlue(void);
void setColor(uint8_t  redValue, uint8_t  greenValue, uint8_t  blueValue);

#endif // !__LED_HPP__
