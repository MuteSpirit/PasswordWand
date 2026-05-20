#include <Arduino.h>
//#include <EEPROM.h>                                                             // for reading and writing AtMega32u4 internal EEprom
#include <SHA256.h>                                                             // for hashing the master password
#include <AES.h>                                                                // for encrypting credentials

#include "encrypt.hpp"
#include "led.hpp"
#include "layout.hpp"
#include "pass_wand_eeprom.hpp"

//- Object setup

//AESSmall256 aes;                                                              // 32 byte key, 32 byte block; this uses 4% more program memory. Set 

void sha256Hash(char *password) {
  for (int i = 0; i < SHA_ITERATIONS; i++) {                                    // only seems to work correctly when SHA_INCREMENTS == 1.
    sha256HashOnce(password);
  }
}

void sha256HashOnce(char *password) {
  SHA256 sha256;

  size_t passwordSize = strlen(password);                                       // strlen(password) == 28
  size_t posn, len;
  uint8_t value[HASHED_MASTER_PASSWORD_SZ];                                     // HASHED_MASTER_PASSWORD_SZ == 32

  sha256.reset();
  for (posn = 0; posn < passwordSize; posn += 1) {                              // posn=0|1|2|3|...
      len = passwordSize - posn;                                                // 28|27|26|25|...
      if (len > passwordSize)
          len = passwordSize;
      sha256.update(password + posn, len);                                      // password[0], 28|password[1],27|password[2],26
  }
  sha256.finalize(value, sizeof(value));
  sha256.clear();
  memcpy(password, value, HASHED_MASTER_PASSWORD_SZ);
}

void encrypt32Bytes(uint8_t *outBuffer, uint8_t *inBuffer) {
  AESSmall128 aes;                                                                // 16 byte key, 32 byte block

  uint8_t leftInBuffer[16];
  uint8_t rightInBuffer[16];

  memcpy(leftInBuffer, inBuffer, 16);
  memcpy(rightInBuffer, inBuffer + 16, 16);
  
  aes.encryptBlock(leftInBuffer, leftInBuffer);
  aes.encryptBlock(rightInBuffer, rightInBuffer);
  
  memcpy(outBuffer, leftInBuffer, 16);
  memcpy(outBuffer + 16, rightInBuffer, 16);
}

void decrypt32(uint8_t *outBuffer, uint8_t *inBuffer) {                         // Necessary because blocksize of AES128/256 = 16 bytes.
  AESSmall128 aes;                                                              // 16 byte key, 32 byte block

  uint8_t leftInBuf[16];
  uint8_t rightInBuf[16];

  memcpy(leftInBuf, inBuffer, 16);
  memcpy(rightInBuf, inBuffer + 16, 16);
  
  aes.decryptBlock(leftInBuf, leftInBuf);                                       // decrypt the buffer 
  aes.decryptBlock(rightInBuf, rightInBuf);                                     // decrypt the buffer 

  memcpy(outBuffer, leftInBuf, 16);
  memcpy(outBuffer + 16, rightInBuf, 16);
}

//- UUID Generation

void setUUID(uint8_t *password, uint8_t size, uint8_t appendNullTerm) {
  for (uint8_t i = 0; i < size; i++) {
    password[i] = random(33,126);                                               // maybe we should use allChars here instead? We're generating PWs w/ chars that we can't input...
                                                                                // 32 = space, 127 = <DEL>, so we want to choose from everything in between.
    if (appendNullTerm) password[size - 1] = NULL_TERM;
  }
}

void setCredSalt(uint8_t *credSalt, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    credSalt[i] = random(0,255);                                                
  }
}
