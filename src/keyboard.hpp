#pragma once

#ifndef __KEYBOARD_HPP__
#define __KEYBOARD_HPP__

#include <inttypes.h>

void keyboard_type_string(const char* s);
void keyboard_push_button(const uint8_t key_code);
void keyboard_push_tab();

#endif // !__KEYBOARD_HPP__
