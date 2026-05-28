#ifndef __EEPROM_STORAGE__
#define __EEPROM_STORAGE__

#include <inttypes.h>

#include "block_storage.hpp"

class InternalEepromStorage : public BlockStorage
{
public:
    bool init();

    virtual bool is_addr_ok(const uint32_t addr) const override;

    virtual uint8_t read(const uint32_t addr) override;
    virtual void write(const uint32_t addr, const uint8_t b) override;

    virtual void read(const uint32_t addr, uint8_t *buf, const uint32_t size) override;
    virtual void write(const uint32_t addr, const uint8_t *buf, const uint32_t size) override;

    virtual void factory_reset() override;

    InternalEepromStorage();
    ~InternalEepromStorage() = default;

protected:
    uint8_t getResetFlag();
    void setResetFlag();

protected:
    unsigned int fullSizeBytes_; // TODO: is it needed to cache that value or use EEPROM.length() directly?
};

#endif // !__EEPROM_STORAGE__

