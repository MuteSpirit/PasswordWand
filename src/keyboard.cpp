#include <Keyboard.h>                                                           // for simulating a USB keyboard and sending output to it
                                                                                //
#include "model.hpp"
#include "ext_storage.hpp"
#include "board.hpp"

#define TAB_KEY                   KEY_TAB                                       // TAB key is ascii 0x2b (not 0x09) or 0x2b, 0xB3; KEY_TAB from Keyboard.h, 0xB3
                                                                                //
uint8_t keyboardFlag;                                                           // flag indicating if we're using the keyboard to edit creds


bool send_username(void);
void send_tab(void);
bool send_password(void);


void
keyboard_setup(void)
{
    pinMode(BTN_NEXT_PIN, INPUT_PULLUP);
    pinMode(BTN_PREV_PIN, INPUT_PULLUP);
    pinMode(BTN_ENTER_PIN, INPUT_PULLUP);
}

void
keyboard_loop_step(void)
{
    if (LOW == digitalRead(BTN_NEXT_PIN)) {
        send_username();
    } 

    if (LOW == digitalRead(BTN_PREV_PIN)) {
        send_password();
    }

    if (LOW == digitalRead(BTN_ENTER_PIN)) {
        send_tab();
    }
}

// bool
// send_username(void)
// {
//     Account acc;
//     if (!ext_eeprom_get(current_account_idx(), acc)) {
//         return false;
//     }
//     Keyboard.begin();                                                             // TODO: can we do a <CTL><A> <BS> here first?  That will clear out pre-populated usernames.
//     Keyboard.print(acc.username);                                                 // type the username through the keyboard, no carriage return
//     Keyboard.end();
// }
//
// void
// send_tab(void)
// {
//     Keyboard.begin();                                                             // TODO: can we do a <CTL><A> <BS> here first?  That will clear out pre-populated usernames.
//     Keyboard.press(TAB_KEY);                                                      // send <TAB>
//     Keyboard.release(TAB_KEY);
//     Keyboard.end();
// }
//
// bool
// send_password(void)
// {                                                           // TODO: can we do a <CTL><A> <BS> here first? That will clear out pre-populated passwords.
//     Account acc;
//     if (!ext_eeprom_get(current_account_idx(), acc)) {
//         return false;
//     }
//     Keyboard.begin();                                                             // TODO: can we do a <CTL><A> <BS> here first?  That will clear out pre-populated usernames.
//     Keyboard.print(acc.username);                                                 // type the username through the keyboard, no carriage return
//     Keyboard.end();
// }
