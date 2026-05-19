#include <Keyboard.h>                                                           // for simulating a USB keyboard and sending output to it
                                                                                //
#include "board.hpp"

void
keyboard_type_string(const char* s)
{
// for Uno it cannot be even compiled because that board cannot emulate keyboard
#ifndef BOARD_ARDUINO_UNO
    Keyboard.begin();
    Keyboard.print(s);
    Keyboard.end();
#endif
}

/// @param key_code - keyboard code from <Keyboard.h>
void
keyboard_push_button(const uint8_t key_code)
{
// for Uno it cannot be even compiled because that board cannot emulate keyboard
#ifndef BOARD_ARDUINO_UNO
    Keyboard.begin();                                                             // TODO: can we do a <CTL><A> <BS> here first?  That will clear out pre-populated usernames.
    Keyboard.press(key_code);                                                      // send <TAB>
    Keyboard.release(key_code);
    Keyboard.end();
#endif
}

/// TAB key is ascii 0x2b (not 0x09) or 0x2b, 0xB3; KEY_TAB from Keyboard.h, 0xB3
void
keyboard_push_tab(void)
{
    keyboard_push_button(KEY_TAB);
}
