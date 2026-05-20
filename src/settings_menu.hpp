#pragma once

#ifndef __SETTINGS_MENU_HPP__
#define __SETTINGS_MENU_HPP__

#include "menu.hpp"
#include "board.hpp"

class SettingsMenu : public Menu
{
public:
    SettingsMenu(Display &oled);
    ~SettingsMenu() {};

    virtual void init() override;

    virtual void activate() override;
    virtual void deactivate() override;

protected:
    Display &oled_;
    bool cli_turn_on_{true};      // TODO: turn off by default
    bool unhide_passwords_{true}; // TODO: turn off by default
};

#endif // !__SETTINGS_MENU_HPP__
