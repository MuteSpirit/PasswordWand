#include <avr/pgmspace.h>
#include <SimpleCLI.h>

#include "cli.hpp"
#include "model.hpp"
#include "ext_storage.hpp"
#include "layout.hpp"


const char * const add_cmd_usage PROGMEM = "Usage: add <username> [password]. Add new account. If \"password\" is absent it'll be generated";
const char * const del_cmd_usage PROGMEM = "Usage: rm <username>. Delete account.";
const char * const ls_cmd_usage  PROGMEM = "Usage: ls. Print exist accounts.";

// Use such long name and not use aliases to avoid mistake during this command usage
const char * const factory_cmd_usage PROGMEM = "Usage: factory_reset. Perform factory reset. !!! All accounts and settings will be lost!!!";

// TODO: SimpleCLI(int commandQueueSize = 10, int errorQueueSize = 10); 
SimpleCLI cli;

struct Account *accts = NULL;
unsigned int accts_size = 0;

void add_account(cmd *pcmd);
void del_account(cmd *pcmd);
void ls_accounts(cmd *pcmd);
void factory_reset(cmd *pcmd);

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

    Command factory_cmd = cli.addCommand("factory_reset", factory_reset);
    factory_cmd.setDescription(factory_cmd_usage);

    cli.setOnError(error_cb);

    // TODO: enable explicitly only
    // cli_off();
    cli_on();
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
add_account(cmd *pcmd)
{
    Command add_cmd(pcmd);

    // TODO: check "isSet" flag of arguments
    Argument name_arg = add_cmd.getArgument(0);
    if (!name_arg.isSet()) {
        Serial.println(F("FAIL: argument <name> is absent"));
        return;
    }

    Argument username_arg = add_cmd.getArgument(1);
    if (!username_arg.isSet()) {
        Serial.println(F("FAIL: argument <username> is absent"));
        return;
    }

    Argument password_arg = add_cmd.getArgument(2);
    // TODO: generate password if user did not pass it

    if (ext_eeprom_is_exist(name_arg.getValue().c_str())) {
        Serial.println(F("FAIL: Account with such name is already exist"));
        return;
    }


    Account acc;
    strncpy(acc.name, name_arg.getValue().c_str(), sizeof(acc.name));
    strncpy(acc.username, username_arg.getValue().c_str(), sizeof(acc.username));
    strncpy(acc.password, password_arg.getValue().c_str(), sizeof(acc.password));

    if (!ext_eeprom_add(acc)) {
        Serial.println(F("FAIL: No space"));
    } else {
        Serial.println(F("OK"));
    }
}

void 
del_account(cmd *pcmd)
{
    Command del_cmd(pcmd);
    Argument name_arg = del_cmd.getArgument(0);
    if (!name_arg.isSet()) {
        Serial.println(F("FAIL: argument <name> is absent"));
    }

    if (!ext_eeprom_del(name_arg.getValue().c_str())) {
        Serial.println(F("FAIL"));
    } else {
        Serial.println(F("OK"));
    }
}

void
ls_accounts(__attribute__((unused)) cmd *pcmd) {
    const uint8_t acc_size = ext_eeprom_get_num_of_accounts();
    uint8_t founded = 0;
    Account acc;
    for (unsigned int i = 0; founded < acc_size && i < CREDS_ACCOMIDATED; ++i) {
        if (!ext_eeprom_get(i, acc)) {
            continue;
        }
        ++founded;
        Serial.println(acc.name); 
        Serial.println(acc.username); 
        Serial.println(acc.password); 
    }
}

void
factory_reset(cmd *pcmd) {
    Command reset_cmd(pcmd);

    if (!storage_factory_reset(Serial)) {
        Serial.println(F("FAIL"));
    } else {
        Serial.println(F("OK"));
    }
}

void
cli_on() {
    cli.unpause();
}

void
cli_off() {
    cli.pause();
}
