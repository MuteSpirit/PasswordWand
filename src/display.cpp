#include <Print.h>

#include "version.hpp"
#include "model.hpp"
#include "ext_storage.hpp"
#include "keyboard.hpp"
#include "accounts_menu.hpp"
#include "settings_menu.hpp"
#include "board.hpp"

AccountsMenu accMenu(oled);
SettingsMenu settingsMenu(oled);;

void switch_to_settings_menu(void *ctx);
void switch_to_accounts_menu(void *ctx);

void
display_setup(void)
{
    oled.init();

    oled.clear();
    oled.home();
}

void
menu_setup(void)
{
    oled.clear();
    oled.home();

    accMenu.init();
    settingsMenu.init();

    switch_to_accounts_menu(NULL);
}

void
switch_to_settings_menu(void *ctx)
{
    (void)(ctx);

    accMenu.deactivate();

    settingsMenu.activate();

    set_button_callback(DeviceButton::button_triangle, switch_to_accounts_menu, NULL);
}

void
switch_to_accounts_menu(void *ctx)
{
    (void)(ctx);

    settingsMenu.deactivate();

    accMenu.activate();

    set_button_callback(DeviceButton::button_triangle, switch_to_settings_menu, NULL);
}
