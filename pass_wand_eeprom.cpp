#include <EEPROM_SPI_WE.h>

#include "pass_wand_eeprom.hpp"
#include "layout.hpp"

                                                                                //
// for Arduino Uno
const int csPin PROGMEM = PD5;
const int wpPin PROGMEM = 999; /* fake. On board it's pulled up to 5V */

EEPROM_SPI_WE eep(csPin, wpPin, 1000000);

//- SPI logic                                                                      from "Make: AVR Programming", Chapter 16. SPI, by Elloit Williams, Published by Maker Media, Inc, 2014
                                                                                // https://www.safaribooksonline.com/library/view/make-avr-programming/9781449356484/ch16.html
// void initSPI(void) {
//   SPI_SS_PRIMARY_DDR |= (1 << SPI_SS_PRIMARY);                                  // set SS output for primary EEprom chip
//   SPI_SS_PRIMARY_DDR |= (1 << SPI_SS_PRIMARY);                                  // set SS output for primary EEprom chip
//   SPI_SS_PRIMARY_PORT |= (1 << SPI_SS_PRIMARY);                                 // start off not selected (high)
//
//   SPI_SS_SECONDARY_DDR |= (1 << SPI_SS_SECONDARY);                              // set SS output for backup EEprom chip
//   SPI_SS_SECONDARY_DDR |= (1 << SPI_SS_SECONDARY);                              // set SS output for backup EEprom chip
//   SPI_SS_SECONDARY_PORT |= (1 << SPI_SS_SECONDARY);                             // start off not selected (high)
//
//   SPI_MOSI_DDR |= (1 << SPI_MOSI);                                              // output on MOSI
//   SPI_MISO_PORT |= (1 << SPI_MISO);                                             // pullup on MISO
//   SPI_SCK_DDR |= (1 << SPI_SCK);                                                // output on SCK
//
//                                                                                 // Don't have to set phase, polarity b/c default works with 25LCxxx chips
// //  SPCR |= (1 << SPR1);                                                        // original coment said this was "div 16, safer for breadboards", but it looks like div 64
//   SPCR |= (1 << SPR0);                                                          // div 16 (if alone)
//   SPCR |= (1 << SPR1);                                                          // div 128 (with line above)
//   //SPCR |= (1 << SPI2X);                                                       // add this to double the rate to div 8, pg. 183 of https://www.pjrc.com/teensy/atmega32u4.pdf
//   SPCR |= (1 << MSTR);                                                          // clockmaster
//   SPCR |= (1 << SPE);                                                           // enable
// }
//
// void SPI_tradeByte(uint8_t byte) {
//   SPDR = byte;                                                                  // SPI starts sending immediately
//   loop_until_bit_is_set(SPSR, SPIF);                                            // wait until done
//                                                                                 // SPDR now contains the received byte
// }


bool 
eeprom_is_addr_ok(const uint16_t addr) {
    return ((addr >= MIN_AVAIL_ADDR) && (addr <= MAX_AVAIL_ADDR));                // minimum and maximum allowed values, false otherwise.
}
                                                                                // When returning false, nothing gets written to eeprom.
bool 
eeprom_write_bytes(const uint16_t startAddr,                                 // TODO: cut out a lot of the boundry checking to reduce the size of this function
                        const uint8_t* buf,
                        const uint8_t numBytes)
{
  // both first byte and last byte addresses must fall within
  // the allowed range 
  if (!eeprom_is_addr_ok(startAddr) || 
      !eeprom_is_addr_ok(startAddr + numBytes)) {
    return false;
  }

  write_eeprom_array(startAddr, buf, (numBytes > EEPROM_BYTES_PER_PAGE) ? EEPROM_BYTES_PER_PAGE : numBytes);
  return true;
}

void eeprom_write_int_bytes( unsigned int addr,                         // given a start address, a buffer and a byte count,
                             const uint8_t* buf,                        // writes the buffer to EEprom
                             uint8_t size)
{
  for (uint16_t i = 0; i < size && eeprom_is_addr_ok(addr + i); i++) {                                      // iterate over every byte in the buffer
    eep.write(addr + i, buf[i]);                                        // write out each byte
  }
}

void eeprom_read_int_string(uint16_t addr,                                  // reads a string from the internal EEprom starting from the specified address
                            unsigned char* buf, 
                            uint8_t size)
{
    for (uint16_t i = 0; i < size && eeprom_is_addr_ok(addr + i * sizeof(uint8_t)); ++i) {
        buf[i] = eep.read(addr + i * sizeof(uint8_t));
    }
}

uint8_t read_eeprom_byte(const uint16_t addr) {
    return eep.read(addr);
}

// READ EEPROM bytes
void 
read_eeprom_array(uint16_t addr, 
                  uint8_t *buf, 
                  uint8_t size,
                  uint8_t primaryFlag) 
{
    (void)(primaryFlag);
    uint8_t b;
    for (uint8_t i = 0; i < size && eeprom_is_addr_ok(addr + i * sizeof(uint8_t)); ++i) {
        buf[i] = eep.read(addr + i * sizeof(uint8_t));
    }
}

void write_eeprom_byte(uint16_t address, const uint8_t byte) {
    eep.write(address, byte);
}

void
write_eeprom_array(
    const uint16_t address, 
    const uint8_t * const buf, 
    const uint8_t size)
{
    for (uint16_t i = 0; i < size; ++i) {
        eep.write(address + i * sizeof(uint8_t), buf[i]);
    }
}
