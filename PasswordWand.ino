#include "src/cli.hpp"
#include "src/model.hpp"
#include "src/version.hpp"
#include "src/display.hpp"
#include "src/ext_storage.hpp"

// Baud rate for serial port
#define SERIAL_BAUD_RATE 38400

#define EnableInterrupts()        sei()                                                                                                                                                 
#define DisableInterrupts()       cli() 

Account acct[] = {
  {"1", "u1", "p1"},
  {"2", "u2", "p2"}
};
const uint16_t acct_size PROGMEM = sizeof(acct) / sizeof(acct[0]);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
setup() {
  cli_init(acct, acct_size);
  cli_on(); // TODO: disable by default

  // enable Serial after init CLI to avoid racing between CLI init and CLI usage
  Serial.begin(SERIAL_BAUD_RATE);

  print_welcome(Serial);

  Print& oled = display_init(acct, acct_size);
  print_welcome(oled);

  //DisableInterrupts();
  no_int_ext_storage_init(oled);
  //EnableInterrupts();
}

void
loop() {
  cli_loop_step();
  menu_loop_step();
}
