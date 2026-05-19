#pragma once

#ifndef __SETTINGS_MENU_HPP__
#define __SETTINGS_MENU_HPP__

#include "menu.hpp"



class SettingsMenu : public Menu
{
public:
    SettingsMenu();
    ~SettingsMenu() {};

    virtual void init() override;

    virtual void activate() override;
    virtual void deactivate() override;

protected:

    bool cli_turn_on_{true};      // TODO: turn off by default
    bool unhide_passwords_{true}; // TODO: turn off by default
};

#endif // !__SETTINGS_MENU_HPP__
