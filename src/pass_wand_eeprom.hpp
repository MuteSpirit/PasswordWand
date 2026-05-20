#pragma once

#ifndef __PASS_WAND_EEPROM_HPP__
#define __PASS_WAND_EEPROM_HPP__

#include <inttypes.h>

class EEPROM_SPI_WE;

extern EEPROM_SPI_WE eep; // TODO: move into class to be able work with several external EEPROM chips

bool eeprom_is_addr_ok(const uint32_t addr);

uint8_t read_eeprom_byte(const uint32_t addr);
void read_eeprom_array(const uint32_t addr, uint8_t *buffer, uint8_t sizeOfBuffer);

void write_eeprom_byte(const uint32_t addr, uint8_t byte);
void write_eeprom_array(const uint32_t addr, const uint8_t * const buf, const uint8_t size);
bool eeprom_write_bytes(const uint32_t addr, const uint8_t* buf, const uint8_t size);

#endif // !__PASS_WAND_EEPROM_HPP__
