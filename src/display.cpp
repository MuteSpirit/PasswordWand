#include <GyverOLED.h>
#include <GyverOLEDMenu.h>

#include "version.hpp"
#include "model.hpp"

#define SHOW_SPLASHSCREEN         3000                                          // in microseconds

// Use OLED_NO_BUFFER to keep RAM for another code
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
OledMenu<3, GyverOLED<SSD1306_128x64, OLED_NO_BUFFER>> menu(&oled);


// On Arduino Uno
const int BTN_NEXT_PIN = 2; // NEXT
const int BTN_PREV_PIN = 4; // PREVIOUS / BACK
const int BTN_ENTER_PIN = 7; // ENTER

void menu_item_cb(const int index, const void* val, const byte valType);

bool cli_turn_on = true;

Account *accounts = NULL;
unsigned int account_size = 0;

Print&
display_init(Account accts[], const unsigned int size) {
    oled.init();
    oled.clear();
    oled.home();

    pinMode(BTN_NEXT_PIN, INPUT_PULLUP);
    pinMode(BTN_PREV_PIN, INPUT_PULLUP);
    pinMode(BTN_ENTER_PIN, INPUT_PULLUP);

    accounts = accts;
    account_size = size;

    // Hide menu here to be able print debug messages on display
    menu.showMenu(false);

    menu.onChange(menu_item_cb, true);

    menu.addItem(PSTR("Accounts"));          // index 0
    menu.addItem(PSTR("CLI"), &cli_turn_on); // index 1
    menu.addItem(PSTR("Factory Reset"));     // index 2

    return oled;
}

void
menu_loop_step() {
    if (LOW == digitalRead(BTN_NEXT_PIN)) {
        menu.selectNext();
    } else if (LOW == digitalRead(BTN_PREV_PIN)) {
        menu.selectPrev();
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
            menu.showMenu(false);
            for (unsigned int i = 0; i < account_size; ++i) {
                oled.println(accounts[i].name);
            }

            delay(5000);

            menu.showMenu(true);
            break;
        case 3: // factory reset
            menu.showMenu(false);

            oled.println(F("Reseting..."));

            delay(2000);

            oled.println(F("Done"));

            delay(2000);

            menu.showMenu(true);
            break;
        default: // unknown
            break;
    }
}
