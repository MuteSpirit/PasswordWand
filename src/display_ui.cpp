#include <Print.h>

#include "display_ui.hpp"
#include "board.hpp"

DisplayUI::DisplayUI(Display &oled, AccountsMenu &accMenu, SettingsMenu &settingsMenu)
    : oled_(oled)
    , accMenu_(accMenu)
    , settingsMenu_(settingsMenu)
{}

void
DisplayUI::ui_setup(void)
{
    oled_.clear();
    oled_.home();

    accMenu_.init(switch_to_settings_menu, this);
    settingsMenu_.init(switch_to_accounts_menu, this);

    switch_to_accounts_menu(this);
}

void
switch_to_settings_menu(void *ctx)
{
    DisplayUI* _this = (DisplayUI*)ctx;

    _this->accMenu_.deactivate();
    _this->settingsMenu_.activate();

    set_button_callback(DeviceButton::button_triangle, switch_to_accounts_menu, ctx);
}

void
switch_to_accounts_menu(void *ctx)
{
    DisplayUI* _this = (DisplayUI*)ctx;

    _this->settingsMenu_.deactivate();
    _this->accMenu_.activate();

    set_button_callback(DeviceButton::button_triangle, switch_to_settings_menu, ctx);
}
