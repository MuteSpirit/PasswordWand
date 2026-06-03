#include "SSD1306Ascii.h" // OLED fonts

#include "src/auth.hpp"
#include "src/cli.hpp"
#include "src/model.hpp"
#include "src/model_storage.hpp"
#include "src/ssd1306_oled.hpp"
#include "src/version.hpp"
#include "src/display_ui.hpp"
#include "src/board.hpp"
#include "src/accounts_menu.hpp"
#include "src/settings_menu.hpp"
#include "src/settings.hpp"
#include "src/auth_form.hpp"
#include "src/ssd1306_oled.hpp"
#include "src/model_storage.hpp"
#include "src/eeprom_storage.hpp"
#include "src/encrypted_block_storage.hpp"

// Baud rate for serial port
#define SERIAL_BAUD_RATE 38400

#define SHOW_SPLASHSCREEN 1000 // ms

Settings settings;

// Use OLED_NO_BUFFER to keep RAM for another code
SSD1306oled oled;

DeviceUserInputs userInputs;

PasswordWandAuth authenticator;

EepromStorage eepromStore(EXT_EEPROM_CS_PIN);
EncryptedBlockStorage ebs(eepromStore);
// TODO: move to callback
// assertTrue(ebs.init(encStoreKey, encStoreKeyLen));

ModelStorage<Account> modelStore(ebs);

AuthForm authForm(oled, userInputs, authenticator);
AccountsMenu accMenu(oled, userInputs, settings, modelStore);
SettingsMenu settingsMenu(oled, userInputs, settings);

DisplayUI ui(oled, userInputs, authForm, accMenu, settingsMenu);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
setup()
{
  pinMode(4, INPUT_PULLUP); // DEBUG

  // TODO: check return value
  eepromStore.init(EEPROM_KBITS_256, EEPROM_PAGE_SIZE_64);

  userInputs.setup();

  oled.setup();
  oled.setFont(System5x7);                                                // perfect, slightly smaller than Arial14

  oled.clear();

  // enable Serial after init CLI to avoid racing between CLI init and CLI usage
  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial);

  print_welcome(oled);
  delay(SHOW_SPLASHSCREEN);

  print_welcome(Serial);

  ui.ui_setup();

  cli_init(settings.cli_turn_on_);
}

void
loop()
{
  static bool startup_delay_done = false;
  if (!startup_delay_done) {
    delay(750);
    startup_delay_done = true;
  }

  cli_loop_step();
  userInputs.loop_step();
}
