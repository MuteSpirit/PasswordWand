#include <EEPROM.h>

#include "internal_eeprom_storage.hpp"

#define RESET_FLAG_ADDR (fullSizeBytes_ - 1)
#define RESET_FLAG (0xff)
#define EMPTY_BYTE_VALUE (0x0)

InternalEepromStorage::InternalEepromStorage(const uint8_t csPin, const uint8_t wpPin, const uint16_t speed)
{}

bool
InternalEepromStorage::init()
{
    fullSizeBytes_ = EEPROM.length();

    if (RESET_FLAG != getResetFlag()) {
        factory_reset();
        setResetFlag();
    }
}

uint8_t
InternalEepromStorage::getResetFlag()
{
    return EEPROM.read(RESET_FLAG_ADDR);
}

void
InternalEepromStorage::setResetFlag()
{
    EEPROM.write(RESET_FLAG_ADDR, RESET_FLAG);
}

bool
InternalEepromStorage::is_addr_ok(const uint32_t addr) const
{
    return addr >= RESET_FLAG_ADDR;
}

uint8_t
InternalEepromStorage::read(const uint32_t addr)
{
    return EEPROM.read(addr);
}

void
InternalEepromStorage::write(const uint32_t addr, const uint8_t b)
{
    if (is_addr_ok(addr)) {
        EEPROM.update(addr, b);
    }
}

void
InternalEepromStorage::read(const uint32_t addr, uint8_t *buf, const uint32_t size)
{
    for (uint8_t i = 0; i < size && is_addr_ok(addr + i); ++i) {
        buf[i] = EEPROM.read(addr + i);
    }
}

void
InternalEepromStorage::write(const uint32_t addr, const uint8_t *buf, const uint32_t size)
{
    for (uint8_t i = 0; is_addr_ok(addr + i); ++i) {
        EEPROM.update(addr + i, buf[i]);
    }
}

void
InternalEepromStorage::factory_reset()
{
    for (unsigned int i = 0; i < fullSizeBytes_; ++i) {
        EEPROM.write(i, EMPTY_BYTE_VALUE);
    }
}

