#ifndef __OLED_HPP__
#define __OLED_HPP__

#include <Print.h>

class Oled : public Print
{
public:
   virtual void clear() = 0; 
   virtual void home() = 0; 

   virtual ~Oled() {};

protected:
   Oled() = default;
};

#endif // !__OLED_HPP__
