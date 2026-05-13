#include <SimpleCLI.h>
#include <avr/pgmspace.h>

#define TITLE  "PasswordWand"
#define VERSION "0.1"
#define AUTHOR "Ivan Efimov"

// Baud rate for serial port
#define BAUD_RATE 38400

#define EEPROM_BYTES_PER_PAGE     0x20  // 32. can't exceed 255 (real page size is 64 for 25LC256)
#define ACCOUNT_NAME_SIZE EEPROM_BYTES_PER_PAGE
#define USERNAME_SIZE EEPROM_BYTES_PER_PAGE
#define PASSWORD_SIZE EEPROM_BYTES_PER_PAGE

SimpleCLI cli; // TODO: optimize queues sizes: SimpleCLI cli(COMMAND_QUEUE_SIZE, ERROR_QUEUE_SIZE);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char * const add_cmd_usage PROGMEM = "Usage: add <username> [password]. Add new account. If \"password\" is absent it'll be generated";
const char * const del_cmd_usage PROGMEM = "Usage: rm <username>. Delete account.";
const char * const ls_cmd_usage  PROGMEM = "Usage: ls. Print exist accounts.";

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Account {
  char name[ACCOUNT_NAME_SIZE];
  char username[USERNAME_SIZE];
  char password[PASSWORD_SIZE];
};

Account acct[] = {
  {"1", "u1", "p1"},
  {"2", "u2", "p2"}
};
const uint16_t acct_size PROGMEM = sizeof(acct) / sizeof(acct[0]);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void add_account(cmd *pcmd);
void del_account(cmd *pcmd);
void ls_accounts(cmd *pcmd);

void error_cb(cmd_error* e);

void print_welcome(void);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
setup() {
  Command add_cmd = cli.addBoundlessCommand("a/d/d", add_account);
  add_cmd.setDescription(add_cmd_usage);

  add_cmd.addPositionalArgument("username");
  add_cmd.addPositionalArgument("password");

  Command del_cmd = cli.addSingleArgCmd("r/m,d/e/l", del_account);
  del_cmd.setDescription(del_cmd_usage);

  del_cmd.addPositionalArgument("username");

  Command ls_cmd = cli.addCommand("l/s", ls_accounts);
  ls_cmd.setDescription(ls_cmd_usage);


  cli.setOnError(error_cb);

  Serial.begin(BAUD_RATE);
  print_welcome();
}

void
loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');

        if (input.length() > 0) {
            //Serial.print("# ");
            //Serial.println(input);

            cli.parse(input);
        }
    }
}

void
error_cb(cmd_error* e) {
  CommandError cmdError(e);

  Serial.print(F("ERROR: "));
  Serial.println(cmdError.toString());

  // Print command usage
  if (cmdError.hasCommand()) {
    Serial.print(F("Did you mean \""));
    Serial.print(cmdError.getCommand().toString());
    Serial.println(F("\"?"));
  }
}
void
add_account(cmd *pcmd) {
  Command add_cmd(pcmd);

  for (int i = 0; i < 2 /*argNum*/; ++i) {
    Argument arg = add_cmd.getArgument(i);
    Serial.print(F("Pass: "));
    Serial.println(arg.getValue());
  }
}

void 
del_account(__attribute__((unused)) cmd *pcmd) {

}

void ls_accounts(__attribute__((unused)) cmd *pcmd) {
  for (int i = 0; i < acct_size; ++i) {
    Serial.println(acct[i].name); 
  }
}

void
print_welcome(void) {
  Serial.println(F(TITLE " v" VERSION));
  Serial.println(F(AUTHOR));
  Serial.println(__DATE__);
}
