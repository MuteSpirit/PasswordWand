#ifndef __BLOCK_STORAGE__
#define __BLOCK_STORAGE__

#include <inttypes.h>

class BlockStorage
{
public:
    virtual bool is_addr_ok(const uint32_t addr) const = 0;

    virtual uint8_t read(const uint32_t addr) = 0;
    virtual void write(const uint32_t addr, const uint8_t b) = 0;

    virtual void read(const uint32_t addr, uint8_t *buf, const uint32_t size) = 0;
    virtual void write(const uint32_t addr, const uint8_t *buf, const uint32_t size) = 0;

    virtual void factory_reset() = 0;

    virtual ~BlockStorage() {};

protected:
    BlockStorage() = default;
};

#endif // !__BLOCK_STORAGE__

