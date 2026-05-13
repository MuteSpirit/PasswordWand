#include <avr/pgmspace.h>
#include <SimpleCLI.h>

#include "cli.hpp"
#include "account.hpp"

const char * const add_cmd_usage PROGMEM = "Usage: add <username> [password]. Add new account. If \"password\" is absent it'll be generated";
const char * const del_cmd_usage PROGMEM = "Usage: rm <username>. Delete account.";
const char * const ls_cmd_usage  PROGMEM = "Usage: ls. Print exist accounts.";

// TODO: SimpleCLI(int commandQueueSize = 10, int errorQueueSize = 10); 
SimpleCLI cli;

struct Account *accts = NULL;
unsigned int accts_size = 0;

void add_account(cmd *pcmd);
void del_account(cmd *pcmd);
void ls_accounts(cmd *pcmd);

void error_cb(cmd_error* e);

void
cli_init(struct Account *pacct, const unsigned int size) {
    accts = pacct;
    accts_size = size;

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

    cli_off();
}

void cli_loop_step() {
    // TODO: use next API to enable CLI only after authentication and disable after expiration nonactive timer
    // cli.pause();
    // cli.unpause();
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');

        if (input.length() > 0) {
            Serial.print("# ");
            Serial.println(input);

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

  for (unsigned int i = 0; i < 2 /*argNum*/; ++i) {
    Argument arg = add_cmd.getArgument(i);
    Serial.print(F("Pass: "));
    Serial.println(arg.getValue());
  }
}

void 
del_account(__attribute__((unused)) cmd *pcmd) {
  Command del_cmd(pcmd);

  for (int i = 0; i < 1 /*argNum*/; ++i) {
    Argument arg = del_cmd.getArgument(i);
    Serial.print(F("Username: "));
    Serial.println(arg.getValue());
  }
}

void ls_accounts(__attribute__((unused)) cmd *pcmd) {
  for (unsigned int i = 0; i < accts_size; ++i) {
    Serial.println(accts[i].name); 
  }
}

void cli_on() {
    cli.unpause();
}

void cli_off() {
    cli.pause();
}
