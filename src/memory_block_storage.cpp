#include <string.h>

#include "memory_block_storage.hpp"

#define RESET_FLAG (0xff)

template<size_t fullSizeBytes, size_t pageSizeBytes>
size_t
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::minAddr() const
{
    return 0;
}

template<size_t fullSizeBytes, size_t pageSizeBytes>
size_t
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::maxAddr() const
{
    return fullSizeBytes - 1 /* init flag size */;
}

template<size_t fullSizeBytes, size_t pageSizeBytes>
bool
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::isAddrOk(const size_t addr) const
{
    return minAddr() <= addr && addr < maxAddr();
}

template<size_t fullSizeBytes, size_t pageSizeBytes>
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::MemoryBlockStorage()
    : store_{0}
{
    setResetFlag();
}

template<size_t fullSizeBytes, size_t pageSizeBytes>
uint8_t
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::getResetFlag()
{
    return read(fullSizeBytes - 1);
}

template<size_t fullSizeBytes, size_t pageSizeBytes>
void
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::setResetFlag()
{
    store_[fullSizeBytes - 1] = RESET_FLAG;
}

template<size_t fullSizeBytes, size_t pageSizeBytes>
uint8_t
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::read(const size_t addr) 
{
    return store_[addr];
}

template<size_t fullSizeBytes, size_t pageSizeBytes>
void
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::write(const size_t addr, const uint8_t b) 
{
    if (addr >= fullSizeBytes - 1) {
        return;
    }
    store_[addr] = b;
}

template<size_t fullSizeBytes, size_t pageSizeBytes>
void
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::read(const size_t addr, uint8_t *buf, const size_t size) 
{
    for (uint32_t i = addr, j = 0; i < addr + size && i < fullSizeBytes; ++i, ++j) {
        buf[j] = read(i);
    }
}

template<size_t fullSizeBytes, size_t pageSizeBytes>
void
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::write(const size_t addr, const uint8_t *buf, const size_t size) 
{
    for (uint32_t i = addr, j = 0; j < size && i < fullSizeBytes; ++j, ++i) {
        write(i, buf[j]);
    }
}

template<size_t fullSizeBytes, size_t pageSizeBytes>
void
MemoryBlockStorage<fullSizeBytes, pageSizeBytes>::factoryReset()
{
    memset(store_, 0, fullSizeBytes);
    setResetFlag();
}
