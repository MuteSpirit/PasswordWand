#pragma once
#ifndef __DISPLAY_UI_HPP__
#define __DISPLAY_UI_HPP__

#include "board.hpp"

class AccountsMenu;
class SettingsMenu;
class UserInputs;


class DisplayUI
{
public:
    DisplayUI(Display&, UserInputs &, AccountsMenu&, SettingsMenu&);

    void ui_setup(void);

protected:
    void switch2settingsMenu();
    void switch2accountsMenu();

protected:
    Display& oled_;
    UserInputs &userInputs_;
    AccountsMenu& accMenu_;
    SettingsMenu& settingsMenu_;
};


#endif // !__DISPLAY_UI_HPP__
