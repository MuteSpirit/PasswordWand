#pragma once

#ifndef __MENU_HPP__
#define __MENU_HPP__

class Menu
{
public:
    virtual ~Menu() {};

    virtual void init(void (*switch_menu_cb)(void *ctx), void *swich_menu_ctx) = 0;

    virtual void activate() = 0;
    virtual void deactivate() = 0;
};

#endif // !__MENU_HPP__
