#ifndef __CLI_H__
#define __CLI_H__

struct Account;

void cli_init(struct Account *pacct, const unsigned int acct_size);
void cli_loop_step();

void cli_on();
void cli_off();

#endif // __CLI_H__
