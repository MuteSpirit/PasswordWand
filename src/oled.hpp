#ifndef __OLED_HPP__
#define __OLED_HPP__

#include <Print.h>

class Oled : public Print
{
public:
    virtual void setup() = 0; 

    virtual void clear() = 0; 
    virtual void home() = 0; 

    virtual void setFont(const uint8_t* font) = 0;

    virtual ~Oled() {};

protected:
    Oled() = default;
};

#endif // !__OLED_HPP__
