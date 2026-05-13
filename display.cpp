#include <GyverOLED.h>
#include "version.hpp"
#include "GyverOLEDMenu.h"
#include "account.hpp"

#define SHOW_SPLASHSCREEN         3000                                          // in microseconds

// Use OLED_NO_BUFFER to keep RAM for another code
GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
OledMenu<3, GyverOLED<SSD1306_128x32, OLED_NO_BUFFER>> menu(&oled);


// On Arduino Uno
const int BTN_NEXT_PIN = 2; // NEXT
const int BTN_PREV_PIN = 4; // PREVIOUS / BACK
const int BTN_ENTER_PIN = 7; // ENTER

void menu_item_cb(const int index, const void* val, const byte valType);

boolean cli_turn_on = false;

Account *accounts = NULL;
unsigned int account_size = 0;

void
display_init(Account accts[], const unsigned int size) {
    oled.init();
    oled.setScale(1);

    pinMode(BTN_NEXT_PIN, INPUT_PULLUP);
    pinMode(BTN_PREV_PIN, INPUT_PULLUP);
    pinMode(BTN_ENTER_PIN, INPUT_PULLUP);

    accounts = accts;
    account_size = size;

    menu.onChange(menu_item_cb, true);

    menu.addItem(PSTR("Accounts"));          // index 0
    menu.addItem(PSTR("CLI"), &cli_turn_on); // index 1
    menu.addItem(PSTR("Factory Reset"));     // index 2
}

void
menu_loop_step(void) {
    if (!digitalRead(BTN_NEXT_PIN)) {
        menu.selectNext(false);
        delay(100);
    } else if (!digitalRead(BTN_PREV_PIN)) {
        menu.selectPrev(false);
        delay(100);
    } else if (!digitalRead(BTN_ENTER_PIN)) {
        menu.toggleChangeSelected();
        delay(100);
    }
}

/// Show splash screen on OLED display for some seconds
/// Interrupts must be enabled to allow "delay()" work
void 
print_welcome_oled(void) {
    oled.clear();
    oled.home();
    oled.println(F(TITLE " v" VERSION));
    oled.println(F(AUTHOR));
    oled.println(__DATE__);

    oled.update();

    delay(SHOW_SPLASHSCREEN);

    menu.showMenu(true);
}

void 
menu_item_cb(const int index, const void* val, const byte valType) {
    switch (index) {
        case 0: // accounts
            menu.showMenu(false, false);

            for (unsigned int i = 0; i < account_size; ++i) {
                oled.println(accounts[i].name);
            }

            delay(5000);

            menu.showMenu(true, false);
            break;
        case 2: // factory reset
            menu.showMenu(false, false);

            oled.clear();
            oled.home();
            oled.println(F("Reseting..."));
            oled.update();

            delay(2000);

            oled.println(F("Done"));
            oled.update();

            delay(2000);

            menu.showMenu(true, false);
            break;
        default: // unknown
            break;
    }
}
