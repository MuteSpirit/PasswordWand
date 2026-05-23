#pragma once

#ifndef __SETTINGS_MENU_HPP__
#define __SETTINGS_MENU_HPP__

// #include <Arduino.h>
#include <inttypes.h>

#include "menu.hpp"
#include "board.hpp"
#include "settings.hpp"

// void menu_item_cb(void *ctx, const int index, const void* val, const byte valType);

class SettingsMenu : public Menu
{
public:
    SettingsMenu(Display &oled, volatile Settings& settings);
    ~SettingsMenu() {};

    virtual void init(void (*switch_menu_cb)(void *ctx), void *swich_menu_ctx) override;

    virtual void activate() override;
    virtual void deactivate() override;

protected:
    struct MenuItem // TODO: move to base class
    {
        const char* title_;

        enum Type
        {
            bool_t,
            action_t
        } type_;

        union
        {
            struct
            {
                bool bValue_;
                volatile bool *targetValue_;
            };
            struct
            {
                void (*cb_)(void *ctx);
                void *ctx_;
            };
        };
    };

protected:
    void drawItem();

    static void select_next_item(void *ctx);
    static void select_prev_item(void *ctx);
    static void navigate_item_cb(void *ctx, int direction);
    static void toggle_change_item_cb(void *ctx);

    void selectPrevItem();
    void selectNextItem();
    void toggleChangeItem();

    static void select_next_value(void *ctx);
    static void select_prev_value(void *ctx);
    static void select_value(void *ctx, int direction);

    void selectPrevValue();
    void selectNextValue();

    void enterEditMode();
    void leaveEditMode();

    static void commit_change_cb(void *ctx);
    static void cancel_change_cb(void *ctx);

    void commitChange();
    void cancelChange();

protected:
    Display &oled_;
    volatile Settings &settings_;

    static const uint8_t numItems_{2};
    MenuItem items_[numItems_];
    uint8_t activeItemIdx_{0};
    bool editMode_{false};

    void (*switch_menu_cb_)(void *ctx);
    void *switch_menu_ctx_;
    //
    // friend void menu_item_cb(void *ctx, const int index, const void* val, const byte valType);
};

#endif // !__SETTINGS_MENU_HPP__
