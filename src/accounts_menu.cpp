#include "accounts_menu.hpp"

#include "board.hpp"
#include "keyboard.hpp"
#include "ext_storage.hpp"

static void send_tab(void *ctx);

AccountsMenu::AccountsMenu(Display& oled)
    : oled_(oled)
{
    memset(&acc_, 0, sizeof(acc_));
}

void
AccountsMenu::init()
{
    acc_idx_ = 0;
    memset(&acc_, 0, sizeof(Account));
}

void
AccountsMenu::activate()
{
    set_button_callback(DeviceButton::button_square, send_username, this);
    set_button_callback(DeviceButton::button_cross, send_password, this);
    set_button_callback(DeviceButton::button_circle, send_tab, this);

    set_rotate_callback(navigate_accounts, this);
}

void
AccountsMenu::deactivate()
{
    set_button_callback(DeviceButton::button_square, stub_btn_cb, NULL);
    set_button_callback(DeviceButton::button_cross, stub_btn_cb, NULL);
    set_button_callback(DeviceButton::button_circle, stub_btn_cb, NULL);

    set_rotate_callback(stub_rotate_cb, NULL);

    acc_idx_ = 0;
    memset(&acc_, 0, sizeof(Account));
}


static void
send_tab(void *ctx)
{
    (void)(ctx);
    keyboard_push_tab();
}

void
send_username(void *ctx)
{
    keyboard_type_string(((AccountsMenu*)ctx)->acc_.username);
}

void
send_password(void *ctx)
{
    keyboard_type_string(((AccountsMenu*)ctx)->acc_.password);
}

void
navigate_accounts(void *ctx, int direction)
{
    AccountsMenu* m = ((AccountsMenu*)ctx);
    uint8_t idx = m->acc_idx_;
    if (direction) {
        if (ext_eeprom_get_next(m->acc_idx_, m->acc_, idx) ||
            ext_eeprom_get_prev(m->acc_idx_, m->acc_, idx)) { 
            m->acc_idx_ = idx;
            m->draw_acc();
        } else {
            if (ext_eeprom_get_prev(m->acc_idx_, m->acc_, idx) ||
                ext_eeprom_get_next(m->acc_idx_, m->acc_, idx)) {
                m->acc_idx_ = idx;
                m->draw_acc();
            }
        }
    }
}

void
AccountsMenu::draw_acc()
{
    
}
