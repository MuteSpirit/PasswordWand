#include "pass_wand_eeprom.hpp"
#include "layout.hpp"
#include "board.hpp"

bool
eeprom_is_addr_ok(const uint32_t addr) {
    return ((addr >= MIN_AVAIL_ADDR) && (addr <= MAX_AVAIL_ADDR));
}

bool
eeprom_write_bytes(const uint32_t addr,
                   const uint8_t* buf,
                   const uint8_t numBytes)
{
    write_eeprom_array(addr, buf, (numBytes > EEPROM_BYTES_PER_PAGE) ? EEPROM_BYTES_PER_PAGE : numBytes);
    return true;
}

uint8_t
read_eeprom_byte(const uint32_t addr)
{
    return eep.read(addr);
}

// READ EEPROM bytes
void
read_eeprom_array(const uint32_t addr,
                  uint8_t *buf,
                  uint8_t size)
{
    for (uint8_t i = 0; i < size && eeprom_is_addr_ok(addr + i); ++i) {
        buf[i] = eep.read(addr + i);
    }
}

void
write_eeprom_byte(const uint32_t addr, const uint8_t byte)
{
    eep.write(addr, byte);
}

void
write_eeprom_array(const uint32_t addr,
                   const uint8_t * const buf,
                   const uint8_t size)
{
    for (uint8_t i = 0; i < size; ++i) {
        eep.write(addr + i, buf[i]);
    }
}
