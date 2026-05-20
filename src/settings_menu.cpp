#include "settings_menu.hpp"

#include "board.hpp"

// There is array "MENU_BOOLEAN_TEXT" defined in next header. 
// And it maybe included by only one one cpp file to avoid symbol redefinition
#include <GyverOLEDMenu.h>

#include "ext_storage.hpp"

typedef OledMenu<4, Display> PassWandOLEDMenu;

PassWandOLEDMenu menu_(&oled);

void menu_item_cb(const int index, const void* val, const byte valType);

SettingsMenu::SettingsMenu(Display &oled)
    : oled_(oled)
{
    // Hide menu here to be able print debug messages on display
    menu_.showMenu(false);

    menu_.onChange(menu_item_cb, true);

    menu_.addItem(PSTR("CLI"), &cli_turn_on_); // index 0
    menu_.addItem(PSTR("Unhide passwords"), &unhide_passwords_);     // index 1
    menu_.addItem(PSTR("Factory Reset"));     // index 2
    menu_.addItem(PSTR("Logout"));     // index 3
}

void
SettingsMenu::init()
{}

void
select_next_item(void *ctx)
{
    ((PassWandOLEDMenu*)ctx)->selectNext();
}

void
select_prev_item(void *ctx)
{
    ((PassWandOLEDMenu*)ctx)->selectPrev();
}

void
select_item_cb(void *ctx, int direction)
{
    if (direction) {
        select_next_item(ctx);
    } else {
        select_prev_item(ctx);
    }
}

void
exec_item_cb(void *ctx)
{
    ((PassWandOLEDMenu*)ctx)->toggleChangeSelected();
}

void
SettingsMenu::activate()
{
    set_button_callback(DeviceButton::button_circle, exec_item_cb, &menu_);

    set_button_callback(DeviceButton::button_square, select_prev_item, &menu_);
    set_button_callback(DeviceButton::button_cross, select_next_item, &menu_);

    set_rotate_callback(select_item_cb, &menu_);
    oled_.home();
    menu_.showMenu(true);
    oled_.update();
}

void
SettingsMenu::deactivate()
{
    menu_.showMenu(false);
    set_button_callback(DeviceButton::button_circle, stub_btn_cb, NULL);

    set_button_callback(DeviceButton::button_square, stub_btn_cb, NULL);
    set_button_callback(DeviceButton::button_cross, stub_btn_cb, NULL);

    set_rotate_callback(stub_rotate_cb, NULL);
    oled_.clear();
}

void 
menu_item_cb(const int index, const void* val, const byte valType) {
    (void)(val);
    (void)(valType);

    switch (index) {
        case 2: // factory reset
            oled.print(F("Reseting..."));
            storage_factory_reset(oled);
            oled.println(F("Done"));
            break;
        case 3:
            oled.print(F("Fake logout..."));
            oled.println(F("Done"));
            break;
        default: // unknown
            break;
    }
}
