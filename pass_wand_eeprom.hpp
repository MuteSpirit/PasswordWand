#pragma once

#ifndef __PASS_WAND_EEPROM_HPP__
#define __PASS_WAND_EEPROM_HPP__

#include <inttypes.h>

class EEPROM_SPI_WE;

extern EEPROM_SPI_WE eep;

// void initSPI(void);
// void SPI_tradeByte(uint8_t byte);                                               // Generic.  Just loads up HW SPI register and waits
void EEPROM_send16BitAddress(uint16_t address);                                 // splits 16-bit address into 2 bytes, sends both
uint8_t EEPROM_readStatus(void);                                                // reads the EEPROM status register
uint8_t EEPROM_readStatusSecondary(void);
void EEPROM_writeEnable(void);                                                  // helper: sets EEPROM write enable
uint8_t read_eeprom_byte(uint16_t address);                                     // gets a byte from a given memory location
void read_eeprom_array( uint16_t address, 
                        uint8_t *buffer, 
                        uint8_t sizeOfBuffer,
                        uint8_t primaryFlag);
void write_eeprom_byte(uint16_t address, uint8_t byte) ;                        // writes a byte to a given memory location
void write_eeprom_array(uint16_t address, 
                        uint8_t *buffer, 
                        uint8_t sizeOfBuffer);
bool eeprom_is_addr_ok(unsigned int addr);
bool eeprom_write_bytes( unsigned int startAddr, 
                            const uint8_t* buf, 
                            uint8_t numBytes);
void eeprom_write_int_bytes( unsigned int startAddr,
                                const uint8_t* buf,
                                uint8_t numBytes);
void eeprom_read_int_string( unsigned int addr,
                             unsigned char* buffer, 
                             uint8_t bufSize);


#endif // !__PASS_WAND_EEPROM_HPP__
