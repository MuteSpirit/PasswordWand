#if defined(EPOXY_DUINO)
#include "encrypted_block_storage.cpp"
#include "encrypted_block_storage.hpp"
#include "memory_block_storage.hpp"
#include <AUnit.h>

test(enc_store_ctor)
{
    MemoryBlockStorage<128, 16> bs;
    EncryptedBlockStorage store(bs);
};
#endif // EPOXY_DUINO
