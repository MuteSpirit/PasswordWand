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

    oled_.home();

    acc_idx_ = 0;
    memset(&acc_, 0, sizeof(Account));

    if (ext_eeprom_get(acc_idx_, acc_) || 
        ext_eeprom_get_next(acc_idx_, acc_, acc_idx_)) {
        draw_acc();
    } else {
        oled_.println(F("No one account founded"));
    }
    oled_.update();
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

    oled_.clear();
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

/// @param[in] direction - 0 = No rotation, 1 = Clockwise, -1 = Counter Clockwise
void
navigate_accounts(void *ctx, int direction)
{
    if (0 == direction) {
        return;
    }

    AccountsMenu* m = ((AccountsMenu*)ctx);
    uint8_t idx = m->acc_idx_;

    do {
        if (direction > 0) {
            if (ext_eeprom_get_next(m->acc_idx_, m->acc_, idx)) {
                break;
            }
            if (ext_eeprom_get(0, m->acc_)) {
                idx = 0;
                break;
            }
            if (ext_eeprom_get_next(0, m->acc_, idx)) {
                break;
            }
        } else {
            if (ext_eeprom_get_prev(m->acc_idx_, m->acc_, idx)) {
                break;
            }
            if (ext_eeprom_get(CREDS_ACCOMIDATED, m->acc_)) {
                idx = CREDS_ACCOMIDATED;
                break;
            }
            if (ext_eeprom_get_prev(CREDS_ACCOMIDATED, m->acc_, idx)) {
                break;
            }
        }
    } while (0);

    if (idx != m->acc_idx_) {
        m->acc_idx_ = idx;

        m->oled_.clear();
        m->oled_.home();

        m->draw_acc();
    }
}

void
AccountsMenu::draw_acc()
{
    oled_.println(acc_.name);
    oled_.println(acc_.username);
    oled_.println(acc_.password);
}
