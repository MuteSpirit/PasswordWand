#include "eeprom_storage.hpp"

#define RESET_FLAG (0xff)

EepromStorage::EepromStorage(const uint8_t csPin, const uint8_t wpPin, const uint16_t speed)
    : eep_(csPin, wpPin, speed)
{}

bool
EepromStorage::init(const enum eeprom_size_t fullSizeBytes, const enum EEPROM_WE_PAGE_SIZE pageSize)
{
    if (!eep_.init()) {
        return false;
    }

    eep_.setPageSize(pageSize);
    eep_.setMemorySize(fullSizeBytes);

    pageSize_ = 16 << pageSize;
    fullSizeBytes_ = (uint32_t)fullSizeBytes;

    if (RESET_FLAG != getResetFlag()) {
        factory_reset();
        setResetFlag();
    }
}

uint8_t
EepromStorage::getResetFlag()
{
    return eep_.read(fullSizeBytes_ - 1);
}

void
EepromStorage::setResetFlag()
{
    eep_.write(fullSizeBytes_ - 1, RESET_FLAG);
}

bool
EepromStorage::is_addr_ok(const uint32_t addr) const
{
    return addr < fullSizeBytes_ - 1 /* init flag size */;
}

uint8_t
EepromStorage::read(const uint32_t addr)
{
    return eep_.read(addr);
}

void
EepromStorage::write(const uint32_t addr, const uint8_t b)
{
    if (is_addr_ok(addr)) {
        eep_.write(addr, b);
    }
}

void
EepromStorage::read(const uint32_t addr, uint8_t *buf, const uint32_t size)
{
    for (uint8_t i = 0; i < size && is_addr_ok(addr + i); ++i) {
        buf[i] = eep_.read(addr + i);
    }
}

void
EepromStorage::write(const uint32_t addr, const uint8_t *buf, const uint32_t size)
{
    for (uint8_t i = 0; is_addr_ok(addr + i); ++i) {
        eep_.write(addr + i, buf[i]);
    }
}

void
EepromStorage::factory_reset()
{
    eep_.eraseCompleteEEPROM();
}
