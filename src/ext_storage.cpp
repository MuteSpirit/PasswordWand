#include <avr/pgmspace.h>
#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>                                                             // for reading and writing AtMega32u4 internal EEprom
#include <EEPROM_SPI_WE.h>
#include <string.h>

#include "ext_storage.hpp"
#include "layout.hpp"
#include "pass_wand_eeprom.hpp"
#include "model.hpp"

#define ADC_READ_PIN              18                                            // we read the voltage from this floating pin to seed the random number generator, don't ground it!

#define EnableInterrupts()        sei()
#define DisableInterrupts()       cli()

#define getResetFlag              read_eeprom_byte(GET_ADDR_RESET_FLAG)

bool get_free_account_pos(uint8_t &pos);
uint8_t count_accounts();

void write_reset_flag(uint8_t buf);

void init_external_eeprom(const uint8_t init_byte);
void init_internal_eeprom(const uint8_t init_byte);

uint8_t acctPosition = 0;                                                       // the positon of the selected account.
uint8_t acctCount = 0;                                                          // the number of accounts in EEprom.

#define BASIC_STR(x) #x
#define STR(x) BASIC_STR(x)

void ext_storage_init(Print& log) {
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
        log.print(F("Factory Reset..."));
        storage_factory_reset(log);
        log.println(F("Ok"));
    } else {
        log.println(F("EEPROM is Ok"));
    }

    acctCount = 0;
    acctPosition = 0;

    acctCount = count_accounts();
    log.print(acctCount);
    log.println(F(" accounts"));
}

static bool
is_free_account_addr(const uint16_t addr)
{
    return (INITIAL_MEMORY_STATE_BYTE == read_eeprom_byte(addr));
}

static bool
mark_free_account_addr(const uint16_t addr)
{
    write_eeprom_byte(addr, INITIAL_MEMORY_STATE_BYTE);
    return true;
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
        // TODO: add encryption
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

uint8_t 
ext_eeprom_get_num_of_accounts(void)
{
    return acctCount;
}


bool
ext_eeprom_get_next(const uint8_t from, Account& acc, uint8_t &idx)
{
    uint16_t addr = MIN_AVAIL_ADDR;

    for (uint8_t i = from + 1; i < CREDS_ACCOMIDATED; ++i) {
        addr = GET_ADDR_ACCT(i);

        if (!is_free_account_addr(addr)) {
            idx = i;
            ext_eeprom_get(i, acc);
            return true;
        }
    }
    return false;
}

bool
ext_eeprom_get_prev(const uint8_t from, Account& acc, uint8_t &idx)
{
    uint16_t addr = MIN_AVAIL_ADDR;

    for (uint8_t i = from >= 1 ? from - 1 : 0; i >= 0; --i) {
        addr = GET_ADDR_ACCT(i);

        if (!is_free_account_addr(addr)) {
            idx = i;
            ext_eeprom_get(i, acc);
            return true;
        }
    }
    return false;
}


bool
storage_factory_reset(Print &log)
{
    init_external_eeprom(INITIAL_MEMORY_STATE_BYTE); // sets all of memory = INITIAL_MEMORY_STATE_BYTE, 0xFF/255/0b11111111
    log.print(F("."));
    init_internal_eeprom(INITIAL_MEMORY_STATE_BYTE); // initialize internal EEprom
    log.print(F("."));
    write_reset_flag(MEMORY_INITIALIZED_FLAG);         // setting the last byte in external EEprom to 0x01 signals that all other 
    log.print(F("."));
    acctCount = 0;
    return true;
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
    return true;
}

void write_reset_flag(uint8_t buf) {                                              // writes the value of the reset flag to EEprom
    write_eeprom_byte(GET_ADDR_RESET_FLAG, buf);
}
                                                                                // This function is used by the other, higher-level functions
void init_internal_eeprom(const uint8_t init_byte) {                                                    // Initializes all of internal EEprom; sets every address = 255.
  for (uint16_t addr = MIN_AVAIL_INT_ADDR; addr <= MAX_AVAIL_INT_ADDR; ++addr) {
    EEPROM.write(addr, init_byte); // TODO: do a bulk write to improve speed
  }
}
