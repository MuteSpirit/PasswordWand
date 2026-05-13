#pragma once

#ifndef __DISPLAY_HPP__
#define __DISPLAY_HPP__

struct Account;

void display_init(Account accts[], const unsigned int size);
void print_welcome_oled(void);
void menu_loop_step(void);

#endif // __DISPLAY_HPP__
