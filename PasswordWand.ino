#include "src/cli.hpp"
#include "src/model.hpp"
#include "src/version.hpp"
#include "src/display.hpp"
#include "src/ext_storage.hpp"
#include "src/board.hpp"

// Baud rate for serial port
#define SERIAL_BAUD_RATE 38400

#define SHOW_SPLASHSCREEN 2000 // ms


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
setup()
{
  board_setup();
  display_setup();

  cli_init();

  // enable Serial after init CLI to avoid racing between CLI init and CLI usage
  Serial.begin(SERIAL_BAUD_RATE);

  print_welcome(Serial);

  print_welcome(oled);
  delay(SHOW_SPLASHSCREEN);

  ext_storage_setup(oled);

  menu_setup();

  // TODO: disable by default
  // cli_off();
  cli_on();
}

void
loop()
{
  cli_loop_step();
  board_loop_step();
}
