#pragma once

#ifndef __MENU_HPP__
#define __MENU_HPP__

class Menu
{
public:
    virtual ~Menu() {};

    virtual void init() = 0;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
};

#endif // !__MENU_HPP__
