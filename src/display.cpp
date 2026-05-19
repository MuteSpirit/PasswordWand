#include <Print.h>

#include "version.hpp"
#include "model.hpp"
#include "ext_storage.hpp"
#include "keyboard.hpp"
#include "accounts_menu.hpp"
#include "settings_menu.hpp"

AccountsMenu accMenu(oled);
SettingsMenu settingsMenu;

void switch_to_settings_menu(void *ctx);
void switch_to_accounts_menu(void *ctx);

Print&
display_init()
{
    oled.init();
    oled.clear();
    oled.home();

    accMenu.init();
    settingsMenu.init();

    switch_to_accounts_menu(NULL);

    return oled;
}

void
switch_to_settings_menu(void *ctx)
{
    (void)(ctx);

    accMenu.deactivate();
    oled.clear();

    settingsMenu.activate();
    oled.update();

    set_button_callback(DeviceButton::button_triangle, switch_to_accounts_menu, NULL);
}

void
switch_to_accounts_menu(void *ctx)
{
    (void)(ctx);

    settingsMenu.deactivate();
    oled.clear();

    accMenu.activate();
    oled.update();

    set_button_callback(DeviceButton::button_triangle, switch_to_settings_menu, NULL);
}
