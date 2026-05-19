// Using Uno only for develop part of features.
// Arduino Pro Micro is target device.
#define BOARD_ARDUINO_UNO

#include "src/cli.hpp"
#include "src/model.hpp"
#include "src/version.hpp"
#include "src/display.hpp"
#include "src/ext_storage.hpp"
#include "src/board.hpp"

// Baud rate for serial port
#define SERIAL_BAUD_RATE 38400

#define SHOW_SPLASHSCREEN 3000 // ms


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
setup()
{
  cli_init();

  // enable Serial after init CLI to avoid racing between CLI init and CLI usage
  Serial.begin(SERIAL_BAUD_RATE);

  print_welcome(Serial);

  Print& oled = display_init();
  print_welcome(oled);
  delay(SHOW_SPLASHSCREEN);

  ext_storage_init(oled);

  board_setup();
}

void
loop()
{
  cli_loop_step();
  board_loop_step();
}
