#include "encrypted_block_storage.hpp"

#include <SHA256.h>

EncryptedBlockStorage::EncryptedBlockStorage(BlockStorage &bs)
    : bs_(bs)
{
}

/// Returns false if the key length is not supported, or the key is somehow "weak" and unusable by this cipher.
bool
EncryptedBlockStorage::init(const uint8_t *key, const uint8_t keySize)
{
    return aes_.setKey(key, keySize);
}

bool
EncryptedBlockStorage::isAddrOk(const size_t addr) const
{
    return bs_.isAddrOk(addr);
}

uint8_t
EncryptedBlockStorage::read(const size_t addr)
{
    if (decryptIdx_ < 0  // no decryption before
        || (blockStartAddr_ + 16 <= addr || addr < blockStartAddr_))
    { // we should perform another decryption
        blockStartAddr_ = (addr / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
        bs_.read(blockStartAddr_, inputBlock_, AES_BLOCK_SIZE);
        aes_.decryptBlock(decryptedBlock_, inputBlock_);
    }

    decryptIdx_ = addr - blockStartAddr_;

    return decryptedBlock_[decryptIdx_];
}

void
EncryptedBlockStorage::write(const size_t addr, const uint8_t b)
{
    (void)(addr);
    (void)(b);
    // Let's not support that
}

void
EncryptedBlockStorage::read(const size_t addr, uint8_t *buf, const size_t size)
{
    (void)(addr);
    (void)(buf);
    (void)(size);
    // Find an encrypted block  for "addr"
}

void
EncryptedBlockStorage::write(const size_t addr, const uint8_t *buf, const size_t size)
{
    (void)(addr);
    (void)(buf);
    (void)(size);
}

void
EncryptedBlockStorage::factoryReset()
{
    bs_.factoryReset();
}
