#include "settings_menu.hpp"

// GyverOLEDMenu library draw menu item ugly and will be replaced
// #define MENU_SELECTED_H  (-5)
// #define MENU_ITEM_PADDING_TOP 10
// #include "GyverOLEDMenu.h"

#include "board.hpp"
#include "cli.hpp"
#include "ext_storage.hpp"

SettingsMenu::SettingsMenu(Display &oled, volatile Settings& settings)
    : oled_(oled)
    , settings_(settings)
{
    uint8_t i = 0;

    items_[i].title_ = "CLI";
    items_[i].type_ = MenuItem::Type::bool_t;
    items_[i].bValue_ = settings_.cli_turn_on_;
    items_[i].targetValue_ = &settings_.cli_turn_on_;

    items_[++i].title_ = "Unhide Pass";
    items_[i].type_ = MenuItem::Type::bool_t;
    items_[i].bValue_ = settings_.unhide_passwords_;
    items_[i].targetValue_ = &settings_.unhide_passwords_;

    // TODO: add "factory reset" and "logout"
}

void
SettingsMenu::init(void (*switch_menu_cb)(void *ctx), void *switch_menu_ctx)
{
    switch_menu_cb_ = switch_menu_cb;
    switch_menu_ctx_ = switch_menu_ctx;
}

void
SettingsMenu::select_next_item(void *ctx)
{
    ((SettingsMenu*)ctx)->selectNextItem();
}

void
SettingsMenu::selectNextItem()
{
    // Serial.print(F("SelectNextItem: "));
    if (activeItemIdx_ < numItems_ - 1) {
        ++activeItemIdx_;
        // Serial.println(activeItemIdx_);
        drawItem();
    }
}

void
SettingsMenu::select_prev_item(void *ctx)
{
    ((SettingsMenu*)ctx)->selectPrevItem();
}

void
SettingsMenu::selectPrevItem()
{
    // Serial.print(F("SelectPrev: "));
    // Serial.println(activeItemIdx_);
    if (activeItemIdx_ > 0) {
        --activeItemIdx_;
        drawItem();
    }
}

void
SettingsMenu::navigate_item_cb(void *ctx, int direction)
{
    if (0 == direction) {
        return;
    }
    if (direction > 0) {
        ((SettingsMenu*)ctx)->selectNextItem();
    } else {
        ((SettingsMenu*)ctx)->selectPrevItem();
    }
}

void
SettingsMenu::toggle_change_item_cb(void *ctx)
{
    ((SettingsMenu*)ctx)->toggleChangeItem();
}

void
SettingsMenu::select_next_value(void *ctx)
{
    ((SettingsMenu*)ctx)->selectNextValue();
}

void
SettingsMenu::select_prev_value(void *ctx)
{
    ((SettingsMenu*)ctx)->selectPrevValue();
}

void
SettingsMenu::select_value(void *ctx, int direction)
{
    if (0 == direction) {
        return;
    }
    if (direction > 0) {
        ((SettingsMenu*)ctx)->selectNextValue();
    } else {
        ((SettingsMenu*)ctx)->selectPrevValue();
    }
}

void
SettingsMenu::toggleChangeItem()
{
    if (editMode_) {
        leaveEditMode();
    } else {
        enterEditMode();
    }
}

void
SettingsMenu::enterEditMode()
{
    editMode_ = true;

    oled.setFont(cp437font8x8);

    set_button_callback(DeviceButton::button_square, SettingsMenu::select_prev_value, this);
    set_button_callback(DeviceButton::button_cross, SettingsMenu::select_next_value, this);

    set_button_callback(DeviceButton::button_circle, SettingsMenu::commit_change_cb, this);
    set_button_callback(DeviceButton::button_triangle, SettingsMenu::cancel_change_cb, this);

    set_rotate_callback(SettingsMenu::select_value, this);

    drawItem();
}

void
SettingsMenu::leaveEditMode()
{
    editMode_ = false;

    oled.setFont(System5x7);

    set_button_callback(DeviceButton::button_square, SettingsMenu::select_prev_item, this);
    set_button_callback(DeviceButton::button_cross, SettingsMenu::select_next_item, this);

    set_button_callback(DeviceButton::button_circle, SettingsMenu::toggle_change_item_cb, this);
    set_button_callback(DeviceButton::button_triangle, switch_menu_cb_, switch_menu_ctx_);

    set_rotate_callback(SettingsMenu::navigate_item_cb, this);

    drawItem();
}

void
SettingsMenu::selectPrevValue()
{
    items_[activeItemIdx_].bValue_ = !items_[activeItemIdx_].bValue_;
    drawItem();
}

void
SettingsMenu::selectNextValue()
{
    items_[activeItemIdx_].bValue_ = !items_[activeItemIdx_].bValue_;
    drawItem();
}

void
SettingsMenu::commit_change_cb(void *ctx)
{
    ((SettingsMenu*)ctx)->commitChange();
}

void
SettingsMenu::cancel_change_cb(void *ctx)
{
    ((SettingsMenu*)ctx)->cancelChange();
}

void
SettingsMenu::commitChange()
{
    if (*items_[activeItemIdx_].targetValue_ != items_[activeItemIdx_].bValue_) {
        if (0 == activeItemIdx_) { // TODO: make action per menu item
            if (items_[activeItemIdx_].bValue_) {
                cli_on();
            } else {
                cli_off();
            }
        }
    }

    *items_[activeItemIdx_].targetValue_ = items_[activeItemIdx_].bValue_;
    leaveEditMode();
}

void
SettingsMenu::cancelChange()
{
    items_[activeItemIdx_].bValue_ = *items_[activeItemIdx_].targetValue_;
    leaveEditMode();
}

void
SettingsMenu::drawItem()
{
    oled_.clear();
    oled_.home();

    if (editMode_) {
        oled_.println(F("<edit mode>"));
    }
    oled_.println(items_[activeItemIdx_].title_);
    oled_.println(F(""));
    // TODO: handle value type
    oled_.println(items_[activeItemIdx_].bValue_ ? "On" : "Off");
}

void
SettingsMenu::activate()
{
    // Serial.println(F("SettingsMenu::activate()"));
    cancelChange();
}

void
SettingsMenu::deactivate()
{
    // Serial.println(F("SettingsMenu::deactivate()"));

    set_button_callback(DeviceButton::button_circle, stub_btn_cb, nullptr);
    set_button_callback(DeviceButton::button_triangle, switch_menu_cb_, switch_menu_ctx_);

    set_button_callback(DeviceButton::button_square, stub_btn_cb, nullptr);
    set_button_callback(DeviceButton::button_cross, stub_btn_cb, nullptr);

    set_rotate_callback(stub_rotate_cb, nullptr);

    oled_.clear();

    editMode_ = false;
    oled.setFont(System5x7);                                                // perfect, slightly smaller than Arial14
}
