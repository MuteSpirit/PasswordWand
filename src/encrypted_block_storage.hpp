#ifndef __ENCRYPTED_BLOCK_STORAGE__
#define __ENCRYPTED_BLOCK_STORAGE__

#include <inttypes.h>
#include <AES.h>

#include "block_storage.hpp"

#define AES_BLOCK_SIZE 16


class EncryptedBlockStorage : public BlockStorage
{
public:
    EncryptedBlockStorage(BlockStorage &bs);
    ~EncryptedBlockStorage() = default;

public:
    bool init(const uint8_t *key, const uint8_t keySize);

    virtual bool isAddrOk(const size_t addr) const override;

    virtual uint8_t read(const size_t addr) override;
    virtual void write(const size_t addr, const uint8_t b) override;

    virtual void read(const size_t addr, uint8_t *buf, const size_t size) override;
    virtual void write(const size_t addr, const uint8_t *buf, const size_t size) override;

    virtual void factoryReset() override;

protected:
    BlockStorage &bs_; /// underline non encrypted block storage

    AES256 aes_; /// for encrypt/decrypt "bs" content

    uint32_t blockStartAddr_;    /// start address of block which has been decrypted
    uint8_t inputBlock_[AES_BLOCK_SIZE];
    uint8_t decryptedBlock_[AES_BLOCK_SIZE];
    int8_t decryptIdx_{-1};      /// pointer to decryptedBlock_ current position. -1 means that decryption did not happen yet
};

#endif // !__ENCRYPTED_BLOCK_STORAGE__
