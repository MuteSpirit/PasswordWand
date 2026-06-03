#pragma once
#ifndef __SSD1306_OLED_HPP__
#define __SSD1306_OLED_HPP__

#include "oled.hpp"


class SSD1306oled : public Oled
{
public:
   virtual void setup() override; 

   virtual void clear() override; 
   virtual void home() override; 

   void setFont(const uint8_t* font) override;

   virtual size_t write(uint8_t) override;
   virtual size_t write(const uint8_t *buffer, size_t size) override;
};

#endif // !__SSD1306_OLED_HPP__
