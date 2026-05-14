#include "cli.hpp"
#include "account.hpp"
#include "version.hpp"
#include "display.hpp"
#include "ext_storage.hpp"

// Baud rate for serial port
#define BAUD_RATE 38400

#define EnableInterrupts()        sei()                                                                                                                                                 
#define DisableInterrupts()       cli() 

Account acct[] = {
  {"1", "u1", "p1"},
  {"2", "u2", "p2"}
};
const uint16_t acct_size PROGMEM = sizeof(acct) / sizeof(acct[0]);

void print_welcome_serial(void);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
setup() {
  cli_init(acct, acct_size);

  // enable Serial after init CLI to avoid racing between CLI init and CLI usage
  Serial.begin(BAUD_RATE);
  print_welcome_serial();

  Print& oled = display_init(acct, acct_size);
  //print_welcome_oled();

  //DisableInterrupts();
  no_int_ext_storage_init(oled);
  //EnableInterrupts();
}

void
loop() {
  cli_loop_step();
  menu_loop_step();
}

void
print_welcome_serial(void) {
  Serial.println(F(TITLE " v" VERSION));
  Serial.println(F(AUTHOR));
  Serial.println(__DATE__);
}
