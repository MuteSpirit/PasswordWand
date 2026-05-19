#include <avr/pgmspace.h>
#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>                                                             // for reading and writing AtMega32u4 internal EEprom
#include <EEPROM_SPI_WE.h>
#include <string.h>

#include "ext_storage.hpp"
#include "version.hpp"
#include "layout.hpp"
#include "pass_wand_eeprom.hpp"
#include "encrypt.hpp"
#include "led.hpp"
#include "model.hpp"

#define ADC_READ_PIN              18                                            // we read the voltage from this floating pin to seed the random number generator, don't ground it!

#define EnableInterrupts()        sei()
#define DisableInterrupts()       cli()
#define getLoginFailures          read_eeprom_byte(GET_ADDR_LOGIN_FAILURES)
#define getResetFlag              read_eeprom_byte(GET_ADDR_RESET_FLAG)
#define getShowPasswordsFlag      read_eeprom_byte(GET_ADDR_SHOW_PW)

uint8_t masterPassword[MASTER_PASSWORD_SIZE];                                   // this is where we store the master password for the device

#define MAX_LOGIN_FAILURES        10                                            // "Factory Reset" after MAX_LOGIN_FAILURES attempts to login. Gurads against 
uint8_t loginFailures = MAX_LOGIN_FAILURES;                                     // count of the number of consecutive login failures since the last successful 
                                                                                // password entry.
                                                                                // brute force attack.
uint8_t showPasswordsFlag;                                                      // flag indicating if we show passwords via the UI, or hide them.
uint8_t addFlag = false;                                                        // tracks wheter we reached the Add Account menu via the main menu or the find menu,

bool get_free_account_pos(uint8_t &pos);
uint8_t count_accounts();

void writeAllToEEProm(uint8_t *accountName, 
                      uint8_t *username, 
                      uint8_t *password, 
                      uint8_t pos) ;
void countAccounts(void) ;
uint8_t getNextFreeAcctPos(void) ;
void readAcctFromEEProm(uint8_t pos, uint8_t *buf);
void readUserFromEEProm(uint8_t pos, uint8_t *buf);
void readStyleFromEEProm(uint8_t pos, char *buf) ;
void readPassFromEEProm(uint8_t pos, uint8_t *buf);
void readCredSaltFromEEProm(uint8_t pos, uint8_t *buf);
uint8_t getListHeadPosition(void);
uint8_t getNextPtr(uint8_t pos);
uint8_t getPrevPtr(uint8_t pos);
void writeNextPtr(uint8_t pos, uint8_t nextPtr);
void writePrevPtr(uint8_t pos, uint8_t prevPtr);
void writeLoginFailures(void);
void writeResetFlag(uint8_t buf);
void writeShowPasswordsFlag(void);
//void writeKeyboardFlag(void);
void writeListHeadPos(void);
void init_external_eeprom(const uint8_t init_byte);
void init_internal_eeprom(const uint8_t init_byte);

void CopyChip(uint8_t restoreFlag);
void writePointers(uint8_t accountPosition, uint8_t *accountName);
//void FixCorruptLinkedList(void);
void ProcessAttributeInput( uint8_t *attributeName, 
                            uint8_t attributeSize, 
                            char    *menuName, 
                            uint8_t nextPosition,
                            uint8_t acctFlag,
                            uint16_t address          );
void enterAttributeChar(uint8_t *attribute, uint8_t passwordFlag);
void EditAttribute(uint8_t aState, uint8_t pos);
void setKey(uint8_t pos);

int position = 0;                                                               // the position of the rotary encoder, used to navigate menus and enter text.
uint8_t enterPosition = 0;                                                      // when alpha editing w/ rotary encoder, position in the edited word
uint8_t acctPosition;                                                           // the positon of the selected account.
uint8_t acctCount = 0;                                                          // the number of accounts in EEprom.
bool authenticated = false;                                                  // indicates if the correct master password has been provided
unsigned long lastActivityTime;                                                 // used to automatically logout after a period of inactivity
uint8_t iterationCount = 0;                                                     // # of times ProcessEvent() called since last evaluation of lastActivityTime
uint8_t headPosition = 0;                                                           // the head of the doubly linked list that keeps account names sorted
uint8_t tailPosition = 0;                                                           // the tail of the doubly linked list that keeps account names sorted
                                                                                //
#define BASIC_STR(x) #x
#define STR(x) BASIC_STR(x)

void no_int_ext_storage_init(Print& log) {
    // log.print(F("Init ext EEPROM..."));
    if (eep.init()){
        // TODO: statically check next
        // static_assert(EEPROM_BYTES_PER_PAGE == EEPROM_PAGE_SIZE_32, "External EEPROM page size " STR(EEPROM_PAGE_SIZE_32) " is not equal to expected by app model " STR(EEPROM_BYTES_PER_PAGE));
        eep.setPageSize(EEPROM_PAGE_SIZE_32);
        eep.setMemorySize(EEPROM_KBITS_256);

        log.println(F("EEPROM Ok"));
    }
    else{
        log.println(F("EEPROM FAIL"));
    }

    if (getResetFlag != MEMORY_INITIALIZED_FLAG) {                                // if memory has never been initialized, initialize it.
        log.print(F("Reset EEPROM..."));
        // log.print(F("Reset int EEPROM..."));
    //     // DisableInterrupts();
        init_internal_eeprom(INITIAL_MEMORY_STATE_BYTE);
    //     // EnableInterrupts();
        // log.println(F("Ok"));
    //                                                                                 //
        // log.print(F("Reset ext EEPROM..."));
    //     // DisableInterrupts();
        init_external_eeprom(INITIAL_MEMORY_STATE_BYTE); // sets all of memory = INITIAL_MEMORY_STATE_BYTE, 0xFF/255/0b11111111
    //     // EnableInterrupts();
        // log.println(F("Ok"));
    //                                                                                 //
        // log.print(F("Set Init Flag..."));
    //     // DisableInterrupts();
        writeResetFlag(MEMORY_INITIALIZED_FLAG);                                    // setting the last byte in external EEprom to 0x01 signals that all other 
    //     // EnableInterrupts();
    //
        log.println(F("Ok"));
    //     // if (getResetFlag != MEMORY_INITIALIZED_FLAG) {
    //     //     log.println(F("Fail update Reset Flag"));
    //     // }
    } else {
        log.println(F("EEPROM is Ok"));
    }

    acctCount = 0;

    acctCount = count_accounts();
    log.print(acctCount);
    log.println(F(" accounts"));
    //
    // {
    //     Account acc = {"t1", "u1", "p1"};
    //     log.println(acc.name);
    //     ext_eeprom_add(acc);
    // }
    // {
    //     Account acc = {"t2", "u2", "p2"}; 
    //     log.println(acc.name);
    //     ext_eeprom_add(acc);
    // }
    //
    // acctCount = count_accounts();
    // log.print(acctCount);
    // log.println(F(" accounts"));

//
//     // if (getResetFlag != MEMORY_INITIALIZED_FLAG) {                                // if memory has never been initialized, initialize it.
//     //     loginFailures = MAX_LOGIN_FAILURES + 1;                                     // so that a condition inside of EVENT_RESET evaluates to true and the reset 
//     //                                                                                 // (25LC256 comes with 0x00 in every address space)
//     //     FactoryReset();                                                             // the reset event will write 0xFF to the location for the reset flag
//     // };
//     //
//     // loginFailures = getLoginFailures;                                             // getLoginFailures returns a byte.
//     // if (loginFailures == INITIAL_MEMORY_STATE_BYTE ) {                            // if loginFailures has never been written too
//     //     loginFailures = 0;                                                          // set it to zero
//     //     writeLoginFailures();                                                       // and write it to EEprom.
//     // }
//     //
//     // keyboardFlag = false;                                                         // setup the keyboard flag
//     //
//     // flipOnOff(keyboardFlag,SET_KEYBOARD,KBD_MENU_O_POS);
//     //
//     // showPasswordsFlag = getShowPasswordsFlag;                                     // setup the show passwords flag and menu item. (getShowPasswordsFlag returns byte)
//     // if (showPasswordsFlag == INITIAL_MEMORY_STATE_BYTE ) {                        // this should never be true because the reset event sets the show passwords 
//     //     showPasswordsFlag = true;                                                   // flag to a value but, for safety, set the show password flag to ON
//     //     writeShowPasswordsFlag();                                                   // and write it to EEprom.
//     // }
//     // flipOnOff(showPasswordsFlag,SET_SHOW_PASSWORD,SH_PW_MENU_O_POS);              // set the menu item to Show Passwrd ON or Show Passwrd OFF.
//
//     PCICR |= (1 << PCIE0);                                                        // Setup interrupts for rotary encoder
//     PCMSK0 |= (1 << PCINT4) | (1 << PCINT5);                                      
//
//     // lastActivityTime = millis();                                                  // establish the start time for when the device is powered up
//     // authenticated = false;                                                        // we're not authenticated yet!
//     //
//     // headPosition = getListHeadPosition();                                         // read the head of the doubly linked list that sorts by account name
//     // acctPosition = headPosition;                                                  // initally the current account it the head account
//     // countAccounts();                                                              // count the number of populated accounts in EEprom
}

// void FactoryReset() {
//     acctCount = 0;
//     acctPosition = 0;
//     headPosition = 0;
//     tailPosition = 0;
//     uint8_t i;
//     while (i < MASTER_PASSWORD_SIZE) masterPassword[i++] = NULL_TERM;           // write over the master password in memory as soon as possible
//     authenticated = false;                                                      // we're no longer authenticated, we need to re-enter the master password
//                                                                                 //
//     init_external_eeprom();                                                         // sets all of memory = INITIAL_MEMORY_STATE_BYTE, 0xFF/255/0b11111111
//     init_internal_eeprom();                                                      // initialize internal EEprom
//     writeResetFlag(MEMORY_INITIALIZED_FLAG);                                    // setting the last byte in external EEprom to 0x01 signals that all other 
//                                                                                 // memory has been initialized to INITIAL_MEMORY_STATE_BYTE and that Initialize
//                                                                                 // doesn't need to execute at startup.
//     loginFailures = 0;                                                          // set login failures back to zero, this also serves as a flag to indicate if 
//                                                                                 // it's the first power on
//     writeLoginFailures();                                                       // write login failure count back to EEprom
//     showPasswordsFlag = true;                                                   // to match the out of box setting (true / 255)
//     writeShowPasswordsFlag();                                                   // write show passwords flag back to EEprom
//     flipOnOff(showPasswordsFlag,SET_SHOW_PASSWORD,SH_PW_MENU_O_POS);            // set the menu item to Show Passwrd ON or Show Passwrd OFF.
// //  keyboardFlag = false;
// //  writeKeyboardFlag();
// //  flipOnOff(keyboardFlag,SET_KEYBOARD,KBD_MENU_O_POS);                        // set the menu item accordingly
//     accountName[0] = NULL_TERM;
//     password[0] = NULL_TERM;
//     username[0] = NULL_TERM;
// }

static bool
is_free_account_addr(const uint16_t addr)
{
    return (INITIAL_MEMORY_STATE_BYTE == read_eeprom_byte(addr));
}

static bool
mark_free_account_addr(const uint16_t addr)
{
    write_eeprom_byte(addr, INITIAL_MEMORY_STATE_BYTE);
}

// Let's temporary forget about "next pos" and "prev pos" fields in Account data on EEPROM. 
// Double linked list is cool but does not help to debug issue right now
// TODO: use double-linked list on EEPROM?
uint8_t
count_accounts()
{
    uint8_t c = 0;
    for (uint16_t pos = 0, addr = GET_ADDR_ACCT(pos); addr < GET_ADDR_SETTINGS; addr = GET_ADDR_ACCT(++pos)) {
        if (!is_free_account_addr(addr)) {
            ++c;
        }
    }
    return c;
}

// Initializes all of external EEprom; sets every address = 255.
void 
init_external_eeprom(const uint8_t init_byte)
{                                                   
    // uint8_t init_buf[EEPROM_BYTES_PER_PAGE];
    // memset(init_buf, INITIAL_MEMORY_STATE_BYTE, sizeof(init_buf));
    //
    // for (uint16_t pageAddress = MIN_AVAIL_ADDR; pageAddress <= MAX_AVAIL_ADDR; pageAddress += EEPROM_BYTES_PER_PAGE) {
    //     eeprom_write_bytes(pageAddress, init_buf, sizeof(init_buf));
    // }

    // Optimization:
    // * write INITIAL_MEMORY_STATE_BYTE as a 1st byte to each account on EEPROM instead of all memory
    // * TODO: write INITIAL_MEMORY_STATE_BYTE to "prev pos" and "next pos" fields
    // * write INITIAL_MEMORY_STATE_BYTE to the settings section

    // TODO: eep.eraseCompleteEEPROM();

    for (uint16_t pos = 0, addr = GET_ADDR_ACCT(pos); addr < GET_ADDR_SETTINGS; addr = GET_ADDR_ACCT(++pos)) {
        write_eeprom_byte(addr, init_byte);
    }

    for (uint16_t addr = GET_ADDR_SETTINGS; addr <= MAX_AVAIL_ADDR; ++addr) {
        write_eeprom_byte(addr, init_byte);
    }
}

bool
ext_eeprom_add(const Account& acc)
{
    uint8_t acc_pos = 0;
    if (!get_free_account_pos(acc_pos)) {
        return false;
    }

    if (!eeprom_write_bytes(GET_ADDR_ACCT(acc_pos), (const uint8_t*)acc.name,     sizeof(acc.name)) ||
        !eeprom_write_bytes(GET_ADDR_USER(acc_pos), (const uint8_t*)acc.username, sizeof(acc.username)) ||
        !eeprom_write_bytes(GET_ADDR_PASS(acc_pos), (const uint8_t*)acc.password, sizeof(acc.password))) {
        // free account position
        eeprom_write_byte(GET_ADDR_ACCT(acc_pos), INITIAL_MEMORY_STATE_BYTE);
        return false;
    }
    // TODO: "fill next", "prev pos" and salt
    ++acctCount;

    return true;
}

bool
ext_eeprom_is_exist(const char* acc_name)
{
    char name[sizeof(Account::name)];
    uint16_t addr = MIN_AVAIL_ADDR;
    uint8_t iterated = 0;
    for (uint8_t idx = 0; idx < CREDS_ACCOMIDATED && iterated < acctCount; ++idx) {
        addr = GET_ADDR_ACCT(idx);

        if (is_free_account_addr(addr)) {
            continue;
        }
        ++iterated;

        read_eeprom_array(addr, name, sizeof(name));

        if (!strncmp(acc_name, name, sizeof(name))) {
            return true;
        }
    }
    return false;
}

bool
ext_eeprom_del(const char* acc_name)
{
    char name[sizeof(Account::name)];
    uint16_t addr = MIN_AVAIL_ADDR;
    uint8_t iterated = 0;
    for (uint8_t idx = 0; idx < CREDS_ACCOMIDATED && iterated < acctCount; ++idx) {
        addr = GET_ADDR_ACCT(idx);

        if (is_free_account_addr(addr)) {
            continue;
        }
        ++iterated;

        read_eeprom_array(addr, name, sizeof(name));

        if (0 == strncmp(acc_name, name, sizeof(name))) {
            mark_free_account_addr(addr);
            --acctCount;
            return true;
        }
    }
    return false;
}

void writeAllToEEProm(uint8_t *accountName, 
                      uint8_t *username, 
                      uint8_t *password, 
                      uint8_t pos)        {                                     // used by delete account and factory reset.
  eeprom_write_bytes(GET_ADDR_ACCT(pos), accountName, ACCOUNT_SIZE);
  eeprom_write_bytes(GET_ADDR_USER(pos), username, USERNAME_SIZE);
  eeprom_write_bytes(GET_ADDR_PASS(pos), password, PASSWORD_SIZE);
}

uint8_t 
ext_eeprom_get_num_of_accounts(void)
{
    return acctCount;
}

bool
storage_factory_reset(Print &log)
{
    init_external_eeprom(INITIAL_MEMORY_STATE_BYTE); // sets all of memory = INITIAL_MEMORY_STATE_BYTE, 0xFF/255/0b11111111
    log.print(F("."));
    init_internal_eeprom(INITIAL_MEMORY_STATE_BYTE); // initialize internal EEprom
    log.print(F("."));
    writeResetFlag(MEMORY_INITIALIZED_FLAG);         // setting the last byte in external EEprom to 0x01 signals that all other 
    log.print(F("."));
    acctCount = 0;
    return true;
}

void
countAccounts(void)
{                                                          // count all of the account names from EEprom.
  acctCount = 0;
  tailPosition = headPosition;
  uint8_t nextPos = getNextPtr(headPosition);
  if (nextPos == headPosition)   {                                              // defense against having head pointing to itself
    eeprom_write_bytes(GET_ADDR_NEXT_POS(headPosition),
                       INITIAL_MEMORY_STATE_BYTE,
                       NEXT_POS_SIZE                   );
    nextPos = getNextPtr(headPosition);
  }
  while(nextPos != INITIAL_MEMORY_STATE_BYTE) {
    acctCount++;
    tailPosition = nextPos;
    nextPos = getNextPtr(nextPos);
    if(tailPosition == nextPos) {                                               // more defense
      eeprom_write_bytes(GET_ADDR_NEXT_POS(tailPosition),
                         INITIAL_MEMORY_STATE_BYTE,
                         NEXT_POS_SIZE                   );
      nextPos = getNextPtr(tailPosition);
    }
  }
}

uint8_t getNextFreeAcctPos() {                                                  // return the position of the next EEprom location for account name marked empty.
  for(uint8_t acctPos = 0; acctPos <= CREDS_ACCOMIDATED; acctPos++) {
      if (read_eeprom_byte(GET_ADDR_ACCT(acctPos)) == 
          INITIAL_MEMORY_STATE_BYTE                     ) {
        return acctPos;
      }
  }
  return INITIAL_MEMORY_STATE_BYTE;
}

/// assign to 'pos' 
/// @param[out] pos - the position of the ext EEPROM location for account name marked empty. 'pos' will be kept as-is if no free space
/// @return true if founded or false otherwise
bool 
get_free_account_pos(uint8_t &pos) {
  for(uint8_t acctPos = 0; acctPos <= CREDS_ACCOMIDATED; ++acctPos) {
      if (INITIAL_MEMORY_STATE_BYTE == read_eeprom_byte(GET_ADDR_ACCT(acctPos))) {
          pos = acctPos;
          return true;
      }
  }
  return false;
}

bool
ext_eeprom_get(const uint8_t idx, Account& acc)
{
    if (INITIAL_MEMORY_STATE_BYTE == read_eeprom_byte(GET_ADDR_ACCT(idx))) {
        acc.name[0] = NULL_TERM;
        acc.username[0] = NULL_TERM;
        acc.password[0] = NULL_TERM;

        return false;
    }

    read_eeprom_array(GET_ADDR_ACCT(idx), acc.name,     sizeof(acc.name));
    read_eeprom_array(GET_ADDR_USER(idx), acc.username, sizeof(acc.username));
    read_eeprom_array(GET_ADDR_PASS(idx), acc.password, sizeof(acc.password));
    // TODO: add decryption
    // } else {
    //     setKey(pos);
    //     decrypt32(buf, buf);
    // }
    return true;
}

void readAcctFromEEProm(uint8_t pos, uint8_t *buf) {
  if (pos > -1) {
    read_eeprom_array(GET_ADDR_ACCT(pos), buf, ACCOUNT_SIZE, true);
  } else {
    buf[0] = NULL_TERM;
  }
  if (buf[0] == INITIAL_MEMORY_STATE_BYTE) {
    buf[0] = NULL_TERM;                                                         // 8 bit twos complement of 255 or 0xFF
  } else {
    setKey(pos);
    decrypt32(buf, buf);
  }
}

void readUserFromEEProm(uint8_t pos, uint8_t *buf) {
  if (pos > -1) {
    read_eeprom_array(GET_ADDR_USER(pos), buf, USERNAME_SIZE, true);
  } else {
    buf[0] = NULL_TERM;
  }
  if (buf[0] == INITIAL_MEMORY_STATE_BYTE) {
    buf[0] = NULL_TERM;
  } else {
    setKey(pos);
    decrypt32(buf, buf);
  }
}

void readPassFromEEProm(uint8_t pos, uint8_t *buf) {                            // TODO: reduce readPassFromEEProm, readUserFromEEProm and readAcctFromEEProm to a single function.
  if (pos > -1) {
    read_eeprom_array(GET_ADDR_PASS(pos), buf, PASSWORD_SIZE, true);
  } else {
    buf[0] = NULL_TERM;
  }
  if (buf[0] == INITIAL_MEMORY_STATE_BYTE) {
    buf[0] = NULL_TERM;
  } else {
    setKey(pos);
    decrypt32(buf, buf);
  }
}

void readStyleFromEEProm(uint8_t pos, char *buf) {
  if (pos > -1) {
    read_eeprom_array(GET_ADDR_STYLE(pos), buf, STYLE_SIZE, true);
  } else {
    buf[0] = NULL_TERM;
  }
  if (buf[0] == INITIAL_MEMORY_STATE_CHAR) buf[0] = NULL_TERM;
}

void readCredSaltFromEEProm(uint8_t pos, uint8_t *buf) {
  if (pos > -1) {
    read_eeprom_array(GET_ADDR_CRED_SALT(pos), buf, CRED_SALT_SIZE, true);
  } else {
    buf[0] = NULL_TERM;
  }
  if (buf[0] == INITIAL_MEMORY_STATE_CHAR) buf[0] = NULL_TERM;
}

uint8_t getListHeadPosition() {                                                 // returns the position of the first element in the linked list
  uint8_t listHead = read_eeprom_byte(GET_ADDR_LIST_HEAD);
  if (listHead == INITIAL_MEMORY_STATE_BYTE) { 
    listHead = getNextFreeAcctPos();
    headPosition = listHead;
    writeListHeadPos();
  }
  return listHead;
}

uint8_t getNextPtr(uint8_t pos) {                                               // given position, returns the address of the next element in the linked list
  return read_eeprom_byte(GET_ADDR_NEXT_POS(pos));
}

uint8_t getPrevPtr(uint8_t pos) {                                               // given position, returns the position of the previous element in the linked list
  return read_eeprom_byte(GET_ADDR_PREV_POS(pos));
}

void writeNextPtr(uint8_t pos, uint8_t nextPtr) {                               // writes the next pointer to EEprom for position, pos.
  write_eeprom_byte(GET_ADDR_NEXT_POS(pos), nextPtr);
}

void writePrevPtr(uint8_t pos, uint8_t prevPtr) {                               // writes the previous pointer to EEprom for position, pos.
  write_eeprom_byte(GET_ADDR_PREV_POS(pos), prevPtr);
}

void writeLoginFailures() {                                                     // writes the number of login failures to EEprom
  write_eeprom_byte(GET_ADDR_LOGIN_FAILURES, loginFailures);
}

void writeResetFlag(uint8_t buf) {                                              // writes the value of the reset flag to EEprom
  write_eeprom_byte(GET_ADDR_RESET_FLAG, buf);
}

void writeShowPasswordsFlag() {
  write_eeprom_byte(GET_ADDR_SHOW_PW, showPasswordsFlag);
}

void writeListHeadPos() {                                                       // writes the position of the beginning of the linked list to EEprom
  write_eeprom_byte(GET_ADDR_LIST_HEAD, headPosition);
}
                                                                                // This function is used by the other, higher-level functions
                                                                                // to prevent bugs and runtime errors due to invalid addresses.

// void init_external_eeprom(void) {                                                   // Initializes all of external EEprom; sets every address = 255.
//   bool colorRed = true;                                                      // show purple during healthy EEprom initialize
//   uint16_t pageAddress = MIN_AVAIL_ADDR;
//   while (pageAddress <= MAX_AVAIL_ADDR) {
//     if (pageAddress%256==0) {
//       if (colorRed) {
//         setRed();
//         colorRed = false;
//       } else {
//         setBlue();
//         colorRed = true;
//       }
//     }
//     EEPROM_writeEnable();
//     SLAVE_PRIMARY_SELECT;
//     SPI_tradeByte(EEPROM_WRITE);
//     EEPROM_send16BitAddress(pageAddress);
//     for (uint8_t i = 0; i < EEPROM_BYTES_PER_PAGE; i++) {
//       SPI_tradeByte(INITIAL_MEMORY_STATE_BYTE);
//     }
//     SLAVE_PRIMARY_DESELECT;
//     pageAddress += EEPROM_BYTES_PER_PAGE;
//     while (EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS)) {;
//     }
//   }
//   setBlue();
// }

void init_internal_eeprom(const uint8_t init_byte) {                                                    // Initializes all of internal EEprom; sets every address = 255.
  for (uint16_t addr = MIN_AVAIL_INT_ADDR; addr <= MAX_AVAIL_INT_ADDR; ++addr) {
    EEPROM.write(addr, init_byte); // TODO: do a bulk write to improve speed
  }
}

void writePointers(uint8_t accountPosition, uint8_t *accountName) {             // traverse through the linked list finding the right spot to insert this record in the list
  if ((headPosition    == 0) &&
      (tailPosition    == 0) &&
      (accountPosition == 0)   ) {                                              // this is the first element added to the linked list
    writePrevPtr(accountPosition, INITIAL_MEMORY_STATE_BYTE);
    writeNextPtr(accountPosition, INITIAL_MEMORY_STATE_BYTE);
    writeListHeadPos();
    return;
  }
  
  uint8_t acctBuf[ACCOUNT_SIZE];                                                // a buffer large enough to accomodate the account name
  uint8_t currentPosition = headPosition;                                       // pointer to the position we're at as we step through the linked list
  uint8_t prevPosition = getPrevPtr(currentPosition);                           // should always be INTIAL_MEMORY_STATE_BYTE.  This IS necessary.
  readAcctFromEEProm(headPosition, acctBuf);                                    // reading the accountName for the head
  while ((currentPosition != INITIAL_MEMORY_STATE_BYTE   ) && 
         (strncmp(acctBuf, accountName, ACCOUNT_SIZE) < 0)     ) {              // if Return value < 0 then it indicates str1 is less than str2.
    prevPosition = currentPosition;                                             // save prevPosition as currentPosition because we'll eventually step over the element that's > accountPosition
    currentPosition = getNextPtr(currentPosition);                              // move to the next element in the linked list
    readAcctFromEEProm(currentPosition,acctBuf);                                // read that account name from EEprom
  }
  if(currentPosition == headPosition) {                                         // inserting before the first element in the list
    headPosition = accountPosition;
    writeListHeadPos();
  }
  if (currentPosition == INITIAL_MEMORY_STATE_BYTE) {                           // inserting an element at the end of the linked list
    tailPosition = accountPosition;
  }
  writePrevPtr(accountPosition, prevPosition   );                               // insert between prevPosition and currentPosition
  writeNextPtr(accountPosition, currentPosition);
  if (prevPosition != INITIAL_MEMORY_STATE_BYTE) {                              // if we're not the new head
    writeNextPtr(prevPosition, accountPosition);                                // update the next pointer of the previous element with the account position.
  }
  if (currentPosition != INITIAL_MEMORY_STATE_BYTE) {                           // if we're not the next element of the tail
    writePrevPtr(currentPosition, accountPosition);                             // write set the previous pointer of the current element to the account position
  }
}
/*
void FixCorruptLinkedList() {                                                   // Rebuild the linked list to fix any issues with the pointers.  Unimplemented because of space restrictions.
//  DisableInterrupts();
  setRed();
  DisplayLine2("Fixing corrupt");
  headPosition = 0;
  tailPosition = 0;
  for (uint8_t pos = 0; pos <= CREDS_ACCOMIDATED; pos++) {                      // Visit every possible location for a set of creds
    uint8_t buffer[ACCOUNT_SIZE];                                               // a buffer that will accomodate the account name
    buffer[0] = INITIAL_MEMORY_STATE_BYTE;
    readAcctFromEEProm(pos, buffer);                                            // get the name of the account at this position, if any
    if (buffer[0] != INITIAL_MEMORY_STATE_BYTE) {                               // if true then creds have been written to this location
      writePointers(pos, buffer);                                               // set the previous and next pointers on this set of credentials
    }
  }
  writeListHeadPos();
  headPosition = getListHeadPosition();                                         // read the head of the doubly linked list that sorts by account name
  acctPosition = headPosition;
  tailPosition = findTailPosition();                                            // find the tail of the doubly linked list that sorts by account name
  position = 0;
  countAccounts();                                                              // count the number of populated accounts in EEprom
  setGreen();
  DisplayLine2("Fixed corrupt");
//  EnableInterrupts();
}
*/

bool authenticateMaster(uint8_t *enteredPassword) {                          // verify if the master password is correct here
  uint8_t eepromMasterHash[HASHED_MASTER_PASSWORD_SZ];                          // buffer for the hashed master password; salt||masterPassword
  uint8_t enteredMasterHash[HASHED_MASTER_PASSWORD_SZ];                         // holds the unhashed master password after some processing
  uint8_t salt[MASTER_PASSWORD_SIZE];                                           // will hold the salt, read from EEprom or calculated
  uint8_t pos = 0;
  while (enteredPassword[pos++] != NULL_TERM);                                  // make sure the unencrypted password is 16 chars long
  while (pos < (MASTER_PASSWORD_SIZE - 1)) enteredPassword[pos++] = NULL_TERM;  // "           "              " , right padded w/ NULL terminator
  enteredPassword[MASTER_PASSWORD_SIZE - 1] = NULL_TERM;                        // NULL_TERM in index 13 no matter what (TODO: is this necessary?)
  byte aByte = EEPROM.read(GET_ADDR_MASTER_HASH);
  if (aByte == INITIAL_MEMORY_STATE_BYTE){                                      // first time, we need to write instead of read
    setUUID(salt, MASTER_PASSWORD_SIZE, false);                                 // generate a random salt
    eeprom_write_int_bytes(GET_ADDR_SALT, salt, MASTER_PASSWORD_SIZE);          // save the salt to EEprom
    memcpy(eepromMasterHash, salt, MASTER_PASSWORD_SIZE);                       // copy salt into the hashed master password variable
    memcpy(eepromMasterHash + MASTER_PASSWORD_SIZE,                             // concatinate the salt and the master password
           enteredPassword, 
           MASTER_PASSWORD_SIZE                          );
    sha256Hash(eepromMasterHash);                                               // hash the master password in place; pass in 32, get back 16
    eeprom_write_int_bytes(GET_ADDR_MASTER_HASH,                                // only write the first 16 bytes of the hashed master password
                           eepromMasterHash, 
                           HASHED_MASTER_PASSWORD_SZ);                          // write the (hased) master password to EEprom
    setGreen();                                                                 // turn the RGB green to signal the correct password was provided
  } else {                                                                      // (buf != INITIAL_MEMORY_STATE_BYTE) | (ch != INITIAL_MEMORY_STATE_CHAR)
    eeprom_read_int_string(GET_ADDR_MASTER_HASH,                                // read hashed master password from EEprom
                           eepromMasterHash,                                    // to compare against the hash of the salt||entered password.
                           HASHED_MASTER_PASSWORD_SZ);
    eeprom_read_int_string(GET_ADDR_SALT,                                       // read salt from EEprom
                           salt, 
                           MASTER_PASSWORD_SIZE);
    memcpy(enteredMasterHash, salt, MASTER_PASSWORD_SIZE);                      // copy salt into the hashed master password variable
    memcpy(enteredMasterHash + MASTER_PASSWORD_SIZE,                            // concatinate the salt and the master password
           enteredPassword,                                                     // entered password
           MASTER_PASSWORD_SIZE                          );
    sha256Hash(enteredMasterHash);                                              // hash the master salt||entered password
    if (0 == memcmp(enteredMasterHash,
                    eepromMasterHash,
                    HASHED_MASTER_PASSWORD_SZ)) {                               // entered password hash matches master password hash, authenticated
      setGreen();                                                               // turn the RGB green to signal the correct password was provided
      loginFailures = 0;                                                        // reset loginFailues to zero
      writeLoginFailures();                                                     // record loginFailures in EEprom
                                                                                // encrypt a word using the master password as the key
    } else {                                                                    // failed authentication
// Begin: decoy password comment                                                // Following section commented out because decoy logic needs to change to accomodate a hashed master password, and because we're out of space
//        if (0 == strcmp(password,strcat(buff,"FR"))) {                        // check for decoy password; masterPassword + "FR".
//          loginFailures = MAX_LOGIN_FAILURES + 2;                             // to turn this functionality back on we'd need to store a hashed version of masterPassword + "FR"
//          event = EVENT_RESET;                                                // in EEprom for comparison to the input password.
//          ProcessEvent();
//        } else {
// End: decoy password comment
      setRed();                                                                 // turn the RGB red to signal the incorrect password was provided
      loginFailures++;
      writeLoginFailures();
      return false;
    }
  }
  return true;
}                                                                               // and check it against the same word that's stored hashed
                                                                                // in eeprom.  This word is written (hashed) to eeprom the 
                                                                                // first time ever a master password is entered.

#ifdef FEATURE_BACKUP_EEPROM

void CopyChip(uint8_t restoreFlag) {                                            // Make a byte for byte duplicate of the backup external EEprom device
  DisplayLine2("Copying...");
  uint8_t buffer[EEPROM_BYTES_PER_PAGE];                                        // make a buffer the same size as the page size.
  for ( uint16_t address = MIN_AVAIL_ADDR; 
        address < MAX_AVAIL_ADDR; 
        address += EEPROM_BYTES_PER_PAGE) {
    setYellow();
    read_eeprom_array(address, buffer, EEPROM_BYTES_PER_PAGE, !restoreFlag);    // if not restoring we are backing up, read from primary chip
    if(!restoreFlag) {                                                          // if we're backing up
      SLAVE_SECONDARY_SELECT;                                                   // select the secondary/backup chip
    } else {                                                                    // otherwise we are restoring
      SLAVE_PRIMARY_SELECT;                                                     // select the primary chip
    }
    SPI_tradeByte(EEPROM_WREN);                                                 // write enable primary
    if(!restoreFlag) {
      SLAVE_SECONDARY_DESELECT;
    } else {
      SLAVE_PRIMARY_DESELECT;
    }
    if(!restoreFlag) {
      SLAVE_SECONDARY_SELECT;
    } else {
      SLAVE_PRIMARY_SELECT;
    }
    SPI_tradeByte(EEPROM_WRITE);
    EEPROM_send16BitAddress(address);
    for (uint8_t i=0;i<EEPROM_BYTES_PER_PAGE;i++) {                             // write the page out byte for byte to the primary
      SPI_tradeByte(buffer[i]);
    }
    if(!restoreFlag) {
      SLAVE_SECONDARY_DESELECT;
    } else {
      SLAVE_PRIMARY_DESELECT;
    }
    if (!restoreFlag) {
      while (EEPROM_readStatusSecondary() & _BV(EEPROM_WRITE_IN_PROGRESS));
    } else {
      while (EEPROM_readStatus() & _BV(EEPROM_WRITE_IN_PROGRESS));
    }
  }
  headPosition = getListHeadPosition();                                         // read the head of the doubly linked list that sorts by account name
  acctPosition = headPosition;
  countAccounts();                                                              // count the number of populated accounts in EEprom
  setGreen();
  DisplayLine2("Copied");
}

#endif // FEATURE_BACKUP_EEPROM

