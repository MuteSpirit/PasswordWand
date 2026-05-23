#pragma once

#ifndef __DISPLAY_UI_HPP__
#define __DISPLAY_UI_HPP__

#include "board.hpp"
#include "settings_menu.hpp"
#include "accounts_menu.hpp"

void switch_to_settings_menu(void *ctx);
void switch_to_accounts_menu(void *ctx);

class DisplayUI
{
public:
    DisplayUI(Display&, AccountsMenu&, SettingsMenu&);

    void ui_setup(void);

protected:
    friend void switch_to_settings_menu(void *ctx);
    friend void switch_to_accounts_menu(void *ctx);

protected:
    Display& oled_;

    AccountsMenu& accMenu_;
    SettingsMenu& settingsMenu_;
};


#endif // !__DISPLAY_UI_HPP__
