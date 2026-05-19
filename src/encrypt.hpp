#pragma once

#ifndef __ENCRYPT_HPP__
#define __ENCRYPT_HPP__

#include <inttypes.h>

#define SHA_ITERATIONS            1                                             // number of times to hash the master password (won't work w/ more than 1 iteration)

bool authenticateMaster(uint8_t *password);
void sha256Hash(char *password);
void encrypt32Bytes(uint8_t *outBuffer, uint8_t *inBuffer);
void decrypt32(uint8_t *outBuffer, uint8_t *inBuffer);
void sha256HashOnce(char *password);
void setCredSalt(uint8_t *credSalt, uint8_t size);
void setUUID(uint8_t *password, uint8_t size, uint8_t appendNullTerm);

#endif // __ENCRYPT_HPP__
