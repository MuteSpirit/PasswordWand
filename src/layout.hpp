#pragma once

#ifndef __LAYOUT_HPP__
#define __LAYOUT_HPP__

#include <Arduino.h>

//- Special Byte Values

#define INITIAL_MEMORY_STATE_CHAR -1                                            // 11111111 binary twos complement, -1 decimal, 0xFF hex.  When factory fresh all bytes in EEprom memory = 0xFF.
#define INITIAL_MEMORY_STATE_BYTE 0xFF                                          // 11111111 binary twos complement, -1 decimal, 0xFF hex.  When factory fresh all bytes in EEprom memory = 0xFF.
#define NULL_TERM                 0x00                                          // The null terminator, NUL, ASCII 0, or '\0'                 
                                                                                //
//- SPI (for Arduino Pro Micro)
/*
#define SPI_SS_PRIMARY            PB6                                           // chip select primary (copy source)  (SPI)
#define SPI_SS_PRIMARY_PORT       PORTB
#define SPI_SS_PRIMARY_PIN        PINB
#define SPI_SS_PRIMARY_DDR        DDRB
#define SPI_SS_SECONDARY          PD7                                           // chip select seconday (copy target)  (SPI)
#define SPI_SS_SECONDARY_PORT     PORTD                                         // 
#define SPI_SS_SECONDARY_PIN      PIND                                          // 
#define SPI_SS_SECONDARY_DDR      DDRD                                          // 

#define SPI_MOSI                  PB2                                           // mosi (SPI)
#define SPI_MOSI_PORT             PORTB
#define SPI_MOSI_PIN              PINB
#define SPI_MOSI_DDR              DDRB
#define SPI_MISO                  PB3                                           // miso (SPI)
#define SPI_MISO_PORT             PORTB
#define SPI_MISO_PIN              PINB
#define SPI_MISO_DDR              DDRB
#define SPI_SCK                   PB1                                           // clock (SPI)
#define SPI_SCK_PORT              PORTB
#define SPI_SCK_PIN               PINB
#define SPI_SCK_DDR               DDRB
*/

//- SPI (for Arduino Uno)
#define SPI_SS_PRIMARY            PD5                                           // chip select primary (copy source)  (SPI)
#define SPI_SS_PRIMARY_PORT       PORTD
#define SPI_SS_PRIMARY_PIN        PIND
#define SPI_SS_PRIMARY_DDR        DDRD
#define SPI_SS_SECONDARY          PD4                                           // chip select seconday (copy target)  (SPI)
#define SPI_SS_SECONDARY_PORT     PORTD                                         // 
#define SPI_SS_SECONDARY_PIN      PIND                                          // 
#define SPI_SS_SECONDARY_DDR      DDRD                                          // 

#define SPI_MOSI                  PB3                                           // mosi (SPI)
#define SPI_MOSI_PORT             PORTB
#define SPI_MOSI_PIN              PINB
#define SPI_MOSI_DDR              DDRB
#define SPI_MISO                  PB4                                           // miso (SPI)
#define SPI_MISO_PORT             PORTB
#define SPI_MISO_PIN              PINB
#define SPI_MISO_DDR              DDRB
#define SPI_SCK                   PB5                                           // clock (SPI)
#define SPI_SCK_PORT              PORTB
#define SPI_SCK_PIN               PINB
#define SPI_SCK_DDR               DDRB


#define SLAVE_PRIMARY_SELECT      SPI_SS_PRIMARY_PORT &= ~(1<<SPI_SS_PRIMARY);
#define SLAVE_PRIMARY_DESELECT    SPI_SS_PRIMARY_PORT |= (1<<SPI_SS_PRIMARY)
#define SLAVE_SECONDARY_SELECT    SPI_SS_SECONDARY_PORT &= ~(1<<SPI_SS_SECONDARY);
#define SLAVE_SECONDARY_DESELECT  SPI_SS_SECONDARY_PORT |= (1<<SPI_SS_SECONDARY)

// Instruction Set -- from data sheet

#define EEPROM_READ               0b00000011                                    // read memory
#define EEPROM_WRITE              0b00000010                                    // write to memory
#define EEPROM_WREN               0b00000110                                    // write enable
#define EEPROM_RDSR               0b00000101                                    // read status register

                                                                                // EEPROM Status Register Bits -- from data sheet
                                                                                // Use these to parse status register
#define EEPROM_WRITE_IN_PROGRESS  0
#define EEPROM_WRITE_ENABLE_LATCH 1
#define EEPROM_BLOCK_PROTECT_0    2
#define EEPROM_BLOCK_PROTECT_1    3

//- Memory Layout

#define MEMORY_INITIALIZED_FLAG   0x01                                          // signals if memory has been initialized correctly
#define EEPROM_BYTES_PER_PAGE     0x20                                          // 32. can't exceed 255 (real page size is 64 for 25LC256)
#define DISPLAY_BUFFER_SIZE       EEPROM_BYTES_PER_PAGE                         // 32
#define MAX_AVAIL_ADDR            0x7FFF                                        // 32,767. 25LC256 = 256kbits capacity.
#define MIN_AVAIL_ADDR            0x00                                          // assuming we start at the very beginning of EEprom
#define ACCOUNT_SIZE              EEPROM_BYTES_PER_PAGE                         // bytes, put on the 1/2 page boundry
#define USERNAME_SIZE             EEPROM_BYTES_PER_PAGE                         // bytes, put on the 1/2 page boundry
#define CRED_SALT_SIZE            0x02                                          // 2 bytes, a uint16_t.  size of key for aes128 == 16 bytes.  2 bytes will be for salt. range= 0 - 65,535
#define PASSWORD_SIZE             EEPROM_BYTES_PER_PAGE                         // 32 bytes
#define STYLE_SIZE                0x02                                          // bytes, we are storing the null terminator
#define PREV_POS_SIZE             0x01                                          // bytes, datatype byte, no null terminator
#define NEXT_POS_SIZE             0x01                                          // bytes, datatype byte, no null terminator
#define CREDS_TOT_SIZE            0x80                                          // 128.  leaving an extra 26 bytes on the end so we're on the page boundry
#define MASTER_PASSWORD_SIZE      (0x10 - CRED_SALT_SIZE)                       // aes256 keysize = 32 bytes.  aes128 keysize = 16 bytes, aes256 blocksize = 16!, only the first 15 chars are part of the password, the rest are ignored.
#define HASHED_MASTER_PASSWORD_SZ (MASTER_PASSWORD_SIZE * 2)                    // the size of the hashed master password
#define LOGIN_FAILURES_SIZE       1
#define SHOW_PASSWORD_FLAG_SIZE   1
#define LIST_HEAD_SIZE            1
#define GET_ADDR_RESET_FLAG       MAX_AVAIL_ADDR                                // address of the reset flag; when not set to 0x01 indicates that memory hasn't been initialized; 32,768
#define CREDS_ACCOMIDATED         (INITIAL_MEMORY_STATE_BYTE - 1)               // 254 is max for the 25LC256 with the configuration related values stored at the end. Can't exceed 255. TODO: calculate (MAX_AVAIL_ADDR + 1) / CREDS_TOT_SIZE.  Use 254 because 255 == INITIAL_MEMORY_STATE_BYTE.

// Account: name, username, password, style, prev, next, salt
#define GET_ADDR_ACCT(pos)        (MIN_AVAIL_ADDR + (pos * CREDS_TOT_SIZE))
#define GET_ADDR_USER(pos)        (MIN_AVAIL_ADDR + ACCOUNT_SIZE + (pos * CREDS_TOT_SIZE))
#define GET_ADDR_PASS(pos)        (MIN_AVAIL_ADDR + ACCOUNT_SIZE + USERNAME_SIZE + (pos * CREDS_TOT_SIZE))
#define GET_ADDR_STYLE(pos)       (MIN_AVAIL_ADDR + ACCOUNT_SIZE + USERNAME_SIZE + PASSWORD_SIZE + (pos * CREDS_TOT_SIZE))
#define GET_ADDR_PREV_POS(pos)    (MIN_AVAIL_ADDR + ACCOUNT_SIZE + USERNAME_SIZE + PASSWORD_SIZE + STYLE_SIZE + (pos * CREDS_TOT_SIZE))
#define GET_ADDR_NEXT_POS(pos)    (MIN_AVAIL_ADDR + ACCOUNT_SIZE + USERNAME_SIZE + PASSWORD_SIZE + STYLE_SIZE + PREV_POS_SIZE + (pos * CREDS_TOT_SIZE))
#define GET_ADDR_CRED_SALT(pos)   (MIN_AVAIL_ADDR + ACCOUNT_SIZE + USERNAME_SIZE + PASSWORD_SIZE + STYLE_SIZE + PREV_POS_SIZE + NEXT_POS_SIZE + (pos * CREDS_TOT_SIZE))

#define GET_ADDR_SETTINGS         (MAX_AVAIL_ADDR - ((EEPROM_BYTES_PER_PAGE * 2) - 1))// use the last page for storing the settings.  TODO: move this to internal EEprom so it is protected by lock bits. 32,704 (cannot be < 32,640)
#define GET_ADDR_LOGIN_FAILURES   (GET_ADDR_SETTINGS)                           //
#define GET_ADDR_SHOW_PW          (GET_ADDR_SETTINGS + LOGIN_FAILURES_SIZE)     //
#define GET_ADDR_LIST_HEAD        (GET_ADDR_SETTINGS + LOGIN_FAILURES_SIZE + SHOW_PASSWORD_FLAG_SIZE ) // points to the head of the linked list

#define MAX_AVAIL_INT_ADDR        0x03FF                                        // 1,023 is the max address of the EEprom on AtMega32u4
#define MIN_AVAIL_INT_ADDR        0x00                                          // assuming we start at the very beginning of EEprom
#define GET_ADDR_MASTER_HASH      (MAX_AVAIL_INT_ADDR - HASHED_MASTER_PASSWORD_SZ)// store hashed master password near the end of EEprom (sneaky)
#define GET_ADDR_SALT             (GET_ADDR_MASTER_HASH - MASTER_PASSWORD_SIZE) // location of the salt for the SHA hash

#endif // !__LAYOUT_HPP__
