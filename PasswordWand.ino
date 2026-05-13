#include "cli.hpp"
#include "account.hpp"
#include "version.hpp"
#include "display.hpp"

// Baud rate for serial port
#define BAUD_RATE 38400

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

  display_init(acct, acct_size);
  print_welcome_oled();
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
