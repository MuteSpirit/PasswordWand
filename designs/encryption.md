# Encryption

## Introduction

This document clarifies why and how encryption is/will be used in project. At least in v1 (MVP). If there will be ideas for future releases - great.

At current state we assume that nRF52840 Pro Micro board will be used.

## Abbreviations

* HLD - High Level Design (don't mention concrete products/libraries/etc. to use)
* LLD - Low Level Design (as much technical details as needed)
* HW - hardware
* creds - credentials
* TOTP (Time-based One-Time Password)
* HOTP (HMAC-based One-Time Password)
* PIV (Personal Identity Verification) / Smart Card

## HLD

Q: Why we need to encrypt stored data? Why not keep credential accounts in plain text?
A: If it might be 100% sure that nobody steal HW creds manager from owner then it would be good variant. But we cannot.

Q: What situation can happen:
A: Next ones:
* intruder infected creds manager owner PC and install keylogger
* intruder knows master password but has no device
* creds manager is stolen by intruder and he does not know master password
* intruder received content of storage
* creds manager is stolen by intruder and he knows master password
* creds manager is stolen together with you and intruder really seriously asks you the password

Let's think about each of them one by one

### Intruder infected creds manager owner PC and install keylogger

HW creds manager is vulnerable to such type of attack because current main use case is "work as keyboard, type username and password itself into login form". We cannot defence.

If try to imagine that device will start to work not as keyboard but support for example FIDO2/WebAuthn, TOTP, HOTP, PIV, etc. Yes, e.g. in case of using one time passwords it doesn't matter catch keylogger it or not, it'll be expired after short time. And then it'd be another type of device.

It's possibly a point to think, idea for future releases, maybe try to integrate TOTP...

### Intruder knows master password but has no device

Not fail yet but close.

We may try prevent catching the master password by someone else technically:
* hide display behind small walls
* show on display stars instead of already entered symbols
* use sensor buttons to avoid click sound
* (bad idea): make password time/day/etc. dependent - one on Monday, another on Tue and so on. It'll be expired in a day.
* Don't use display at all, e.g. allow to attach bluetooth headset and pronounce currently typing symbol. Therefore intruder will be not able to get know master password by look on display.
* (not enough good idea) Hide typed part by default, show by demand but print symbols rotated, even maybe each symbol should be rotated independently... I guess customer will be able to recognize what password has been already entered but outside human look will break intruder brain. At the same time if intruder will make a photo then he'll have enough time to decode password symbols.

Also we may support change master password feature. And if you guess that somebody knows your master password then just change it.

### Creds manager is stolen by intruder and he does not know master password

Creds manager does not contain master password in plain view. It contains salt and hash.
Salt is needed to append it partly to password.
Hash is needed to confirm that salted password is correct.

Let's assume that intruder received both salt and hash from the device. I guess only hash will be enough to find salted password in rainbow tables. Of course if he has such tables for passwords [33, 64] symbols length. Then it'll be not a problem to decript storage.

So we must not allow anyone to read master password hash from the device.

And looks like it's possible but requires extra work - read this thread https://forum.seeedstudio.com/t/firmware-protection-nrf52840/291651/4

nRF52840 datasheet says about next features:
* ARM Cryptocell CC310 cryptographic accelerator
* 128 bit AES/ECB/CCM/AAR co-processor

Device will be protected, intruder will be not able to read memory, update bootloader or update sketch on device.

If use Cryptocell-310 Key Management Unit then we may even not store hash in internal NVM flash memory - only salt.

Let's change master password verify procedure:
* Receive password from user
* Salt it
* Encrypt some random string using AES256 in ECB mode
  * Reason: ECB mode does not use IV and output for the same input text is stable for each call
* Ask Cryptocell encrypt the same string using key from TrustZone and AES256 in ECB mode
* Compare result

Q: Why we need salt?
A: Reason is simple - AES256 requires key length 32 but we allow user to choose shorter passwords and part of salt will be used to expand internally used password lenght to 32.
Salt is generated randomly to be enough strong.
For first version encrypted accounts will be located at internal storage but it's high chance that ecrypted storage will be on external EEPROM/microSD in other device releases.

### Creds manager is physically stolen together with you and intruder really seriously ask you the password

#### Option 1. Kamikaze mode

Ok, you are in trouble but sure that you'll be alive if all creds will be lost.
In that case feature "Special password for factory reset" will help. Say that special password to intruder and device will perform factory reset.

#### Option 2. Fake facade mode

Let's add next use case:
* on 1st device initialization add another special password.
* if say that password to intruder and auth with it then device will use not real passwords fake instead.
* visually access to accounts will be granted but auth will fail.
* how explain that - it's on you, use some semi-truth.

(+) Passwords will be not lost

Implementation variants:
  * Option 2a: generate randomly on-fly
    * (-) Intruder may find out that password is different each time for the same account
    * (+) Solution is simpler
  * Option 2b: on each add/del/modify account in main storage generate random fake password and keep in parallel non-encrypted storage keeping only passwords
    * (+) Passwords will be persistent
    * (-) This project is OpenSource so intruder may be will have access to storage and

#### Option 3. Device lock mode

Use case: special password locks device for 24 hr

May be it's reasonable to have several passwords which lock device for different periods, e.g. 1 day, 1 week, 1 month...

... or there will be configuration option for lock period.

#### Option 4. Implement all variants

You will be able to follow different scenarios:
1. say wrong password, lost one login try and some health, say password to open fake storage.
2. say wrong password, lost one login try and some health, say password which perform factory reset.
3. say password which perform factory reset. Possible consequences are on you.
4. say password which lock device for "lock period"

# Draft

The primary key management feature available for the nRF52840 is the ability to use a Device Root Key (KDR).

What it is: The KDR is a 128-bit AES key that can be loaded into a special, secure register within the CryptoCell itself. This key can be a randomly generated, unique key for each device.

How it works: Once loaded, this key is used directly by the CryptoCell hardware for cryptographic operations. The key material never leaves the hardware in plaintext, protecting it from software-based attacks that might try to read it from normal system RAM. The key is stored in an "Always on Domain" (AO), meaning it is retained between resets.

## Crypto Alghorithms and Salt

AES128 was used for PasswordPump v1 because of lack of resources but there are another conditions on nRF52840 and we may use more strong AES256.

I thought that salt for each credential account is also needed like a common practice but if use AES with NOT ECB mode then intruder will be not able to find some patterns in encrypted text.
!!! check the Cryptocell capabilities to set AES mode !!!

But if ECB is supported only then salt will be needed as key to encrypt account data (username, password). That will make the same logins/passwords in different accounts unique in encrypted form.

It means that less data should be stored for each account.
