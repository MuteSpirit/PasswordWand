#pragma once
#ifndef __DISPLAY_UI_HPP__
#define __DISPLAY_UI_HPP__

class Oled;
class AccountsMenu;
class SettingsMenu;
class AuthForm;
class UserInputs;


class DisplayUI
{
public:
    DisplayUI(Oled&, UserInputs &, AuthForm &, AccountsMenu&, SettingsMenu&);

    void ui_setup(void);

protected:
    void switch2settingsMenu();
    void switch2accountsMenu();

protected:
    Oled& oled_;
    UserInputs &userInputs_;
    AuthForm &authForm_;
    AccountsMenu& accMenu_;
    SettingsMenu& settingsMenu_;
};


#endif // !__DISPLAY_UI_HPP__
