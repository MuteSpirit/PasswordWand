#ifndef __CLI_H__
#define __CLI_H__

struct Account;

void cli_init(const bool cli_turn_on);
void cli_loop_step();

void cli_on();
void cli_off();

#endif // __CLI_H__
