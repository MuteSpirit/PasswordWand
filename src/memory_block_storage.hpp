#ifndef __MEMORY_BLOCK_STORAGE__
#define __MEMORY_BLOCK_STORAGE__

#include <inttypes.h>
#include "block_storage.hpp"


class MemoryBlockStorageTestHelper;

template<uint32_t fullSizeBytes, uint32_t pageSizeBytes>
class MemoryBlockStorage : public BlockStorage
{
public:
    virtual bool is_addr_ok(const uint32_t addr) const override;

    virtual uint8_t read(const uint32_t addr) override;
    virtual void write(const uint32_t addr, const uint8_t b) override;

    virtual void read(const uint32_t addr, uint8_t *buf, const uint32_t size) override;
    virtual void write(const uint32_t addr, const uint8_t *buf, const uint32_t size) override;

    virtual void factory_reset() override;

    MemoryBlockStorage();
    ~MemoryBlockStorage() = default;

protected:
    uint8_t getResetFlag();
    void setResetFlag();

protected:
    uint8_t store_[fullSizeBytes];
    uint32_t offset_{0};

    friend MemoryBlockStorageTestHelper;
};

#endif // !__MEMORY_BLOCK_STORAGE__
