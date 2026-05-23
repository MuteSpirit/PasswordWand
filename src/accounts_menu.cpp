#include "accounts_menu.hpp"

#include "board.hpp"
#include "keyboard.hpp"
#include "ext_storage.hpp"
#include "model.hpp"

static void send_tab(void *ctx);

AccountsMenu::AccountsMenu(Display& oled, volatile const Settings& settings)
    : oled_(oled)
    , settings_(settings)
{
    memset(&acc_, 0, sizeof(acc_));
}

void
AccountsMenu::init(void (*switch_menu_cb)(void *ctx), void *swich_menu_ctx)
{
    // we don't touch switch_menu callback on "triangle" button so far, so don't remember that
    (void)switch_menu_cb;
    (void)swich_menu_ctx;

    acc_idx_ = 0;
    memset(&acc_, 0, sizeof(Account));
}

void
AccountsMenu::activate()
{
    // Serial.println(F("AccountsMenu::activate()"));
    //
    set_button_callback(DeviceButton::button_square, send_username, this);
    set_button_callback(DeviceButton::button_cross, send_password, this);
    set_button_callback(DeviceButton::button_circle, send_tab, this);

    // DEBUG // set_rotate_callback(navigate_accounts, this);

    oled_.home();

    acc_idx_ = 0;
    memset(&acc_, 0, sizeof(Account));

    // DEBUG
    strcpy(acc_.name, "acc1");
    strcpy(acc_.username, "user1");
    strcpy(acc_.password, "passwd1");

    draw_acc();

    // if (ext_eeprom_get(acc_idx_, acc_) || 
    //     ext_eeprom_get_next(acc_idx_, acc_, acc_idx_)) {
    //     draw_acc();
    // } else {
    //     oled_.println(F("No creds accounts"));
    // }
}

void
AccountsMenu::deactivate()
{
    // Serial.println(F("AccountsMenu::deactivate()"));
    //
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
    oled_.println("  Credentials:  ");

    oled_.print(F("N: "));
    oled_.println(acc_.name);

    oled_.print(F("U: "));
    oled_.println(acc_.username);

    oled_.print(F("P: "));
    if (settings_.unhide_passwords_) {
        oled_.println(acc_.password);
    } else {
        char hidden_passwd[PASSWORD_SIZE] = {0};
        const uint8_t len = strnlen(acc_.password, PASSWORD_SIZE);
        for (uint8_t i = 0; i < len; ++i) {
            hidden_passwd[i] = '*';
        }
        oled_.println(hidden_passwd);
    }
}
