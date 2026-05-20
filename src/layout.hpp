#pragma once

#ifndef __LAYOUT_HPP__
#define __LAYOUT_HPP__

#include <Arduino.h>

//- Special Byte Values

#define INITIAL_MEMORY_STATE_CHAR -1                                            // 11111111 binary twos complement, -1 decimal, 0xFF hex.  When factory fresh all bytes in EEprom memory = 0xFF.
#define INITIAL_MEMORY_STATE_BYTE 0xFFul                                          // 11111111 binary twos complement, -1 decimal, 0xFF hex.  When factory fresh all bytes in EEprom memory = 0xFF.
#define NULL_TERM                 0x00                                          // The null terminator, NUL, ASCII 0, or '\0'                 
                                                                                //
//- Memory Layout

#define MEMORY_INITIALIZED_FLAG   0x01ul                                         // signals if memory has been initialized correctly
#define EEPROM_BYTES_PER_PAGE     0x20ul                                         // 32. can't exceed 255 (real page size is 64 for 25LC256)
#define DISPLAY_BUFFER_SIZE       EEPROM_BYTES_PER_PAGE                         // 32
#define MAX_AVAIL_ADDR            (uint32_t)(0x7FFFul)                           // 32,767. 25LC256 = 256kbits capacity.
#define MIN_AVAIL_ADDR            (uint32_t)(0x00ul)                             // assuming we start at the very beginning of EEprom
#define ACCOUNT_SIZE              EEPROM_BYTES_PER_PAGE                         // bytes, put on the 1/2 page boundry
#define USERNAME_SIZE             EEPROM_BYTES_PER_PAGE                         // bytes, put on the 1/2 page boundry
#define CRED_SALT_SIZE            0x02ul                                         // 2 bytes, a uint16_t.  size of key for aes128 == 16 bytes.  2 bytes will be for salt. range= 0 - 65,535
#define PASSWORD_SIZE             EEPROM_BYTES_PER_PAGE                         // 32 bytes
#define STYLE_SIZE                0x02ul                                         // bytes, we are storing the null terminator
#define PREV_POS_SIZE             0x01ul                                         // bytes, datatype byte, no null terminator
#define NEXT_POS_SIZE             0x01ul                                         // bytes, datatype byte, no null terminator
#define CREDS_TOT_SIZE            0x80ul                                         // 128.  leaving an extra 26 bytes on the end so we're on the page boundry
#define MASTER_PASSWORD_SIZE      (0x10ul - CRED_SALT_SIZE)                      // aes256 keysize = 32 bytes.  aes128 keysize = 16 bytes, aes256 blocksize = 16!, only the first 15 chars are part of the password, the rest are ignored.
#define HASHED_MASTER_PASSWORD_SZ (MASTER_PASSWORD_SIZE * 2)                    // the size of the hashed master password
#define LOGIN_FAILURES_SIZE       1
#define SHOW_PASSWORD_FLAG_SIZE   1
#define LIST_HEAD_SIZE            1
#define GET_ADDR_RESET_FLAG       MAX_AVAIL_ADDR                                // address of the reset flag; when not set to 0x01 indicates that memory hasn't been initialized; 32,768
#define CREDS_ACCOMIDATED         (INITIAL_MEMORY_STATE_BYTE - 1)               // 254 is max for the 25LC256 with the configuration related values stored at the end. Can't exceed 255. TODO: calculate (MAX_AVAIL_ADDR + 1) / CREDS_TOT_SIZE.  Use 254 because 255 == INITIAL_MEMORY_STATE_BYTE.

// Account: name, username, password, style, prev, next, salt
#define GET_ADDR_ACCT(pos)        (uint32_t)(MIN_AVAIL_ADDR + (pos * CREDS_TOT_SIZE))
#define GET_ADDR_USER(pos)        (uint32_t)(MIN_AVAIL_ADDR + ACCOUNT_SIZE + (pos * CREDS_TOT_SIZE))
#define GET_ADDR_PASS(pos)        (uint32_t)(MIN_AVAIL_ADDR + ACCOUNT_SIZE + USERNAME_SIZE + (pos * CREDS_TOT_SIZE))
#define GET_ADDR_STYLE(pos)       (uint32_t)(MIN_AVAIL_ADDR + ACCOUNT_SIZE + USERNAME_SIZE + PASSWORD_SIZE + (pos * CREDS_TOT_SIZE))
#define GET_ADDR_PREV_POS(pos)    (uint32_t)(MIN_AVAIL_ADDR + ACCOUNT_SIZE + USERNAME_SIZE + PASSWORD_SIZE + STYLE_SIZE + (pos * CREDS_TOT_SIZE))
#define GET_ADDR_NEXT_POS(pos)    (uint32_t)(MIN_AVAIL_ADDR + ACCOUNT_SIZE + USERNAME_SIZE + PASSWORD_SIZE + STYLE_SIZE + PREV_POS_SIZE + (pos * CREDS_TOT_SIZE))
#define GET_ADDR_CRED_SALT(pos)   (uint32_t)(MIN_AVAIL_ADDR + ACCOUNT_SIZE + USERNAME_SIZE + PASSWORD_SIZE + STYLE_SIZE + PREV_POS_SIZE + NEXT_POS_SIZE + (pos * CREDS_TOT_SIZE))

#define GET_ADDR_SETTINGS         (uint32_t)(MAX_AVAIL_ADDR - ((EEPROM_BYTES_PER_PAGE * 2) - 1))// use the last page for storing the settings.  TODO: move this to internal EEprom so it is protected by lock bits. 32,704 (cannot be < 32,640)
#define GET_ADDR_LOGIN_FAILURES   (GET_ADDR_SETTINGS)                           //
#define GET_ADDR_SHOW_PW          (GET_ADDR_SETTINGS + LOGIN_FAILURES_SIZE)     //
#define GET_ADDR_LIST_HEAD        (GET_ADDR_SETTINGS + LOGIN_FAILURES_SIZE + SHOW_PASSWORD_FLAG_SIZE ) // points to the head of the linked list

#define MAX_AVAIL_INT_ADDR        (uint32_t)(0x03FFul)                                        // 1,023 is the max address of the EEprom on AtMega32u4
#define MIN_AVAIL_INT_ADDR        (uint32_t)(0x00ul)                                          // assuming we start at the very beginning of EEprom
#define GET_ADDR_MASTER_HASH      (MAX_AVAIL_INT_ADDR - HASHED_MASTER_PASSWORD_SZ)// store hashed master password near the end of EEprom (sneaky)
#define GET_ADDR_SALT             (GET_ADDR_MASTER_HASH - MASTER_PASSWORD_SIZE) // location of the salt for the SHA hash

#endif // !__LAYOUT_HPP__
