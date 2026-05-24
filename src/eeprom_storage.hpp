#ifndef __EEPROM_STORAGE__
#define __EEPROM_STORAGE__

#include <inttypes.h>
#include <Arduino.h>
#include <EEPROM_SPI_WE.h>

#include "block_storage.hpp"

class EepromStorage : public BlockStorage
{
public:
    bool init(const enum eeprom_size_t fullSizeBytes, const enum EEPROM_WE_PAGE_SIZE pageSize);

public:
    virtual bool is_addr_ok(const uint32_t addr) const override;

    virtual uint8_t read(const uint32_t addr) override;
    virtual void write(const uint32_t addr, const uint8_t b) override;

    virtual void read(const uint32_t addr, uint8_t *buf, const uint32_t size) override;
    virtual void write(const uint32_t addr, const uint8_t *buf, const uint32_t size) override;

    virtual void factory_reset() override;

    EepromStorage(const uint8_t csPin, const uint8_t wpPin = 999, const uint16_t speed = F_CPU);
    ~EepromStorage() = default;

protected:
    uint8_t getResetFlag();
    void setResetFlag();

protected:
    EEPROM_SPI_WE eep_;
    uint32_t fullSizeBytes_;
    const uint16_t pageSize_;
};

#endif // !__EEPROM_STORAGE__
