#include "cli.hpp"
#include "account.hpp"

#define TITLE  "PasswordWand"
#define VERSION "0.1"
#define AUTHOR "Ivan Efimov"

// Baud rate for serial port
#define BAUD_RATE 38400

Account acct[] = {
  {"1", "u1", "p1"},
  {"2", "u2", "p2"}
};
const uint16_t acct_size PROGMEM = sizeof(acct) / sizeof(acct[0]);

void print_welcome(void);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
setup() {
  cli_init(acct, acct_size);

  // enable Serial after init CLI to avoid racing between CLI init and CLI usage
  Serial.begin(BAUD_RATE);
  
  print_welcome();
}

void
loop() {
  cli_loop_step();
}

void
print_welcome(void) {
  Serial.println(F(TITLE " v" VERSION));
  Serial.println(F(AUTHOR));
  Serial.println(__DATE__);
}
