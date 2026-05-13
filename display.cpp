#include <GyverOLED.h>
#include <GyverOLEDMenu.h>
#include <RotaryEncoder.h>

#include "version.hpp"
#include "account.hpp"
#include "fsm.hpp"
#include "cli.hpp"

#define SHOW_SPLASHSCREEN         1500 /* ms */

// Use OLED_NO_BUFFER to keep RAM for another code
GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;
OledMenu<3, GyverOLED<SSD1306_128x32, OLED_NO_BUFFER>> main_menu(&oled);
RotaryEncoder encoder(A2, A3); 

// Pins on Arduino Uno
const int BTN_NEXT_PIN  = 2; // NEXT
const int BTN_PREV_PIN  = 6; // PREVIOUS / BACK
const int BTN_ENTER_PIN = 7; // ENTER

void menu_item_cb(const int index, const void* val, const byte valType);

boolean cli_turn_on = false;

Account *accounts = NULL;
unsigned int account_size = 0;

static int encoder_pos = 0;
int encoder_new_pos;

void
display_init(Account accts[], const unsigned int size) {
    oled.init();
    oled.setScale(1);

    pinMode(BTN_NEXT_PIN, INPUT_PULLUP);
    pinMode(BTN_PREV_PIN, INPUT_PULLUP);
    pinMode(BTN_ENTER_PIN, INPUT_PULLUP);

    //////////////////////////////////////
    pinMode(A2, INPUT_PULLUP);
    pinMode(A3, INPUT_PULLUP); 
    //////////////////////////////////////

    accounts = accts;
    account_size = size;

    main_menu.onChange(menu_item_cb, true);

   main_menu.addItem(PSTR("Accounts"));          // index 0
   main_menu.addItem(PSTR("CLI"), &cli_turn_on); // index 1
   main_menu.addItem(PSTR("Factory Reset"));     // index 2
}

void
menu_loop_step(void) {
/*
    if (!digitalRead(BTN_NEXT_PIN)) {
       main_menu.selectNext(false);
        delay(150);
    } else if (!digitalRead(BTN_PREV_PIN)) {
       main_menu.selectPrev(false);
        delay(150);
    } else if (!digitalRead(BTN_ENTER_PIN)) {
       main_menu.toggleChangeSelected();
        delay(150);
    }
*/

    if (!digitalRead(BTN_ENTER_PIN)) {
        if (main_menu.isMenuShowing) {
            main_menu.toggleChangeSelected();
        } else {
            fsm_process(FsmEvent::enterMenuItem);
        }
    }

   encoder.tick();
   encoder_new_pos = encoder.getPosition();

   if (encoder_pos > encoder_new_pos) {
      fsm_process(FsmEvent::nextMenuItem);
   } else if (encoder_pos < encoder_new_pos) {
      fsm_process(FsmEvent::prevMenuItem);
   }

   if (encoder_pos != encoder_new_pos) {
       encoder_pos = encoder_new_pos;
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

   main_menu.showMenu(true);
}

void 
menu_item_cb(const int index, const void* val, const byte valType) {
    switch (index) {
        case 0: // accounts
            fsm_process(FsmEvent::enterMenuItem);
            break;
        case 1: // CLI
            if (NULL == val) {
                break;
            }
            switch (valType) {
                case VAL_BOOLEAN:
                    if (*(boolean*)val) {
                        cli_on();
                    } else {
                        cli_off();
                    }
                    break;
                default:
                    break;
            }
            break;
        case 2: // factory reset
            main_menu.showMenu(false, false);

            oled.clear();
            oled.home();
            oled.println(F("Reseting..."));
            oled.update();

            delay(2000);

            oled.println(F("Done"));
            oled.update();

            delay(2000);

            main_menu.showMenu(true, false);
            break;
        default: // unknown
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
FsmState state = FsmState::mainMenu;
unsigned int accountIdx = 0;

void fsm_jump_to(FsmState s) {
    state = s;
}

void show_account(const Account&);

void 
fsm_process(FsmEvent e) {
    switch (state) {
        case FsmState::mainMenu:
            switch (e) {
                case FsmEvent::nextMenuItem:
                    main_menu.selectNext(false);
                    break;
                case FsmEvent::prevMenuItem:
                    main_menu.selectPrev(false);
                    break;
                case FsmEvent::enterMenuItem:
                    main_menu.showMenu(false, false);
                    accountIdx = 0;

                    show_account(accounts[accountIdx]);

                    state = FsmState::accounts;
                    break;
                case FsmEvent::leaveMenuItem:
                    // there is no parent menu
                    break;
            };
            break;

        case FsmState::accounts:
            switch (e) {
                case FsmEvent::nextMenuItem:
                    if (account_size - 1 == accountIdx) {
                        accountIdx = -1;
                    }
                    show_account(accounts[++accountIdx]);
                    break;
                case FsmEvent::prevMenuItem:
                    if (0 == accountIdx) {
                        accountIdx = account_size;
                    }
                    show_account(accounts[--accountIdx]);
                    break;
                case FsmEvent::enterMenuItem:
                case FsmEvent::leaveMenuItem:
                    main_menu.showMenu(true, false);
                    state = FsmState::mainMenu;
                    break;
            };
            break;

    }
    return ;
}


void show_account(const Account& a) {
    oled.clear();

    oled.home();

    oled.println(a.name);
    oled.println(a.username);
    oled.println(a.password);

    oled.update();
}
