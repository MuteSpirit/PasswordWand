#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#define EEPROM_BYTES_PER_PAGE     0x20  // 32. can't exceed 255 (real page size is 64 for 25LC256)

#define ACCOUNT_NAME_SIZE EEPROM_BYTES_PER_PAGE
#define USERNAME_SIZE     EEPROM_BYTES_PER_PAGE
#define PASSWORD_SIZE     EEPROM_BYTES_PER_PAGE

struct Account {
  char name[ACCOUNT_NAME_SIZE];
  char username[USERNAME_SIZE];
  char password[PASSWORD_SIZE];
};

#endif // !__MODEL_HPP__
