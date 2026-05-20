#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <inttypes.h>

#include "layout.hpp"

#define ACCOUNT_NAME_SIZE EEPROM_BYTES_PER_PAGE
#define USERNAME_SIZE     EEPROM_BYTES_PER_PAGE
#define PASSWORD_SIZE     EEPROM_BYTES_PER_PAGE

struct Account {
  char name[ACCOUNT_NAME_SIZE];
  char username[USERNAME_SIZE];
  char password[PASSWORD_SIZE];
};

#endif // !__MODEL_HPP__
