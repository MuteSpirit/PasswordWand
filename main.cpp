#include "src/cli.hpp"
#include "src/model.hpp"
#include "src/version.hpp"
#include "src/display_ui.hpp"
#include "src/ext_storage.hpp"
#include "src/board.hpp"
#include "src/accounts_menu.hpp"
#include "src/settings_menu.hpp"
#include "src/settings.hpp"

// Baud rate for serial port
#define SERIAL_BAUD_RATE 38400

#define SHOW_SPLASHSCREEN 1000 // ms

Settings settings;

// Use OLED_NO_BUFFER to keep RAM for another code
Display oled;

DeviceUserInputs userInputs;
AccountsMenu accMenu(oled, userInputs, settings);
SettingsMenu settingsMenu(oled, userInputs, settings);

DisplayUI ui(oled, userInputs, accMenu, settingsMenu);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
setup()
{
  userInputs.setup();

  oled.begin(&Adafruit128x32, OLED_I2C_ADDR);
  oled.setFont(System5x7);                                                // perfect, slightly smaller than Arial14
  // oled.init(OLED_SDA_PIN, OLED_SCK_PIN);

  oled.clear();

  // enable Serial after init CLI to avoid racing between CLI init and CLI usage
  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial);

  print_welcome(oled);
  delay(SHOW_SPLASHSCREEN);

  print_welcome(Serial);

  // TODO: restore when possible
  // ext_storage_setup(oled);

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
