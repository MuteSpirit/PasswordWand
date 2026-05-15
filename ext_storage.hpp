#pragma once

#ifndef __EXT_STORAGE_HPP__
#define __EXT_STORAGE_HPP__

#include <Print.h>
#include <inttypes.h>

void no_int_ext_storage_init(Print& log);

struct Account;
bool ext_eeprom_add(const Account& acc);
bool ext_eeprom_get(const uint8_t idx, Account& acc);
uint8_t ext_eeprom_get_num_of_accounts(void);
bool ext_eeprom_is_exist(const char* acc_name);
bool ext_eeprom_del(const char* acc_name);

bool storage_factory_reset(Print& log);

#endif // __EXT_STORAGE_HPP__
