#include "accounts_menu.hpp"

#include "blind_call.hpp"
#include "board.hpp"
#include "keyboard.hpp"
#include "ext_storage.hpp"
#include "model.hpp"
#include "settings.hpp"

AccountsMenu::AccountsMenu(Display& oled, UserInputs &userInputs, const Settings& settings)
    : oled_(oled)
    , userInputs_(userInputs)
    , settings_(settings)
{
    memset(&acc_, 0, sizeof(acc_));
}

void
AccountsMenu::init(BlindCall switchMenuCb)
{
    // we don't touch switch_menu callback on "triangle" button so far, so don't remember that
    (void)(switchMenuCb);

    acc_idx_ = 0;
    memset(&acc_, 0, sizeof(Account));
}

void
AccountsMenu::activate()
{
    userInputs_.set(UserInputs::Button::square, BlindCall::make(this, &AccountsMenu::sendUsername));
    userInputs_.set(UserInputs::Button::cross, BlindCall::make(this, &AccountsMenu::sendPassword));
    userInputs_.set(UserInputs::Button::circle, BlindCall::make(this, &AccountsMenu::sendTab));

    userInputs_.set(UserInputs::Encoder::rotary, BlindCall::make(this,&AccountsMenu::navigateAccounts));

    oled_.home();

    acc_idx_ = 0;
    memset(&acc_, 0, sizeof(Account));

    // DEBUG
    strcpy(acc_.name, "acc1");
    strcpy(acc_.username, "user1");
    strcpy(acc_.password, "passwd1");

    draw();

    // if (ext_eeprom_get(acc_idx_, acc_) || 
    //     ext_eeprom_get_next(acc_idx_, acc_, acc_idx_)) {
    //     draw();
    // } else {
    //     oled_.println(F("No creds accounts"));
    // }
}

void
AccountsMenu::deactivate()
{
    userInputs_.unset(UserInputs::Button::square);
    userInputs_.unset(UserInputs::Button::cross);
    userInputs_.unset(UserInputs::Button::circle);

    userInputs_.unset(UserInputs::Encoder::rotary);

    acc_idx_ = 0;
    memset(&acc_, 0, sizeof(Account));

    oled_.clear();
}


void
AccountsMenu::sendTab()
{
    keyboard_push_tab();
}

void
AccountsMenu::sendUsername()
{
    keyboard_type_string(acc_.username);
}

void
AccountsMenu::sendPassword()
{
    keyboard_type_string(acc_.password);
}

/// @param[in] direction - 0 = No rotation, 1 = Clockwise, -1 = Counter Clockwise
void
AccountsMenu::navigateAccounts(int direction)
{
    if (0 == direction) {
        return;
    }

    uint8_t idx = acc_idx_;

    do {
        if (direction > 0) {
            if (ext_eeprom_get_next(acc_idx_, acc_, idx)) {
                break;
            }
            if (ext_eeprom_get(0, acc_)) {
                idx = 0;
                break;
            }
            if (ext_eeprom_get_next(0, acc_, idx)) {
                break;
            }
        } else {
            if (ext_eeprom_get_prev(acc_idx_, acc_, idx)) {
                break;
            }
            if (ext_eeprom_get(CREDS_ACCOMIDATED, acc_)) {
                idx = CREDS_ACCOMIDATED;
                break;
            }
            if (ext_eeprom_get_prev(CREDS_ACCOMIDATED, acc_, idx)) {
                break;
            }
        }
    } while (0);

    if (idx != acc_idx_) {
        acc_idx_ = idx;

        oled_.clear();
        oled_.home();

        draw();
    }
}

void
AccountsMenu::draw()
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
