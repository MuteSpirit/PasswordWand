#include <string.h>

#include "memory_block_storage.hpp"

#define RESET_FLAG (0xff)

template<uint32_t fullSizeBytes, uint32_t pageSizeBytes>
bool
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::is_addr_ok(const uint32_t addr) const
{
    return addr < fullSizeBytes - 1 /* init flag size */;
}

template<uint32_t fullSizeBytes, uint32_t pageSizeBytes>
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::MemoryBlockStorage()
    : store_{0}
{
    setResetFlag();
}

template<uint32_t fullSizeBytes, uint32_t pageSizeBytes>
uint8_t
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::getResetFlag()
{
    return read(fullSizeBytes - 1);
}

template<uint32_t fullSizeBytes, uint32_t pageSizeBytes>
void
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::setResetFlag()
{
    store_[fullSizeBytes - 1] = RESET_FLAG;
}

template<uint32_t fullSizeBytes, uint32_t pageSizeBytes>
uint8_t
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::read(const uint32_t addr) 
{
    return store_[addr];
}

template<uint32_t fullSizeBytes, uint32_t pageSizeBytes>
void
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::write(const uint32_t addr, const uint8_t b) 
{
    if (addr >= fullSizeBytes - 1) {
        return;
    }
    store_[addr] = b;
}

template<uint32_t fullSizeBytes, uint32_t pageSizeBytes>
void
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::read(const uint32_t addr, uint8_t *buf, const uint32_t size) 
{
    for (uint32_t i = addr, j = 0; i < addr + size && i < fullSizeBytes; ++i, ++j) {
        buf[j] = read(i);
    }
}

template<uint32_t fullSizeBytes, uint32_t pageSizeBytes>
void
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::write(const uint32_t addr, const uint8_t *buf, const uint32_t size) 
{
    for (uint32_t i = addr, j = 0; j < size && i < fullSizeBytes; ++j, ++i) {
        write(i, buf[j]);
    }
}

template<uint32_t fullSizeBytes, uint32_t pageSizeBytes>
void
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::factory_reset()
{
    memset(store_, 0, fullSizeBytes);
    setResetFlag();
}
