# Threats

## Introduction

This document lists threats related to credential manager and possible options to solve them or warning about impossibility to do that.

## HLD. Threats

### Intruder infected PasswordWand's owner PC with keylogger

! Warning ! HW creds manager is vulnerable to such type of attack because current main use case is "work as keyboard, type username and password itself into login form". We cannot defence.

If try to imagine that device will start to work not as keyboard but support for example FIDO2/WebAuthn, TOTP, HOTP, PIV, etc. Yes, e.g. in case of using one time passwords it doesn't matter catch keylogger it or not, it'll be expired after short time. And then it'd be another type of device.

It's possibly a point to think, idea for future releases, maybe try to integrate TOTP...

### Intruder infected PasswordWand's owner PC and can see clipboard buffer content

PasswordWand does not use clipboard buffer for typing username/password/etc. therefore there are no troubles with such use case.

! Warning ! But if User will modify creds via CLI and copy sensitive information into clipboard buffer then Intruder will get them!

### Intruder redirect PasswordWand's owner to phishing site

! Warning ! HW creds manager is vulnerable to such type of attack.

### Intruder knows master password but has no device

Not fail yet but close.

We may support feature "Change master password". And if you guess that somebody knows your master password then just change it.

It's more interesting how Intruder got known User's PasswordWand password...

#### Intruder visually watched auth procedure

For example Intruder uses video camera (personal or one of hacked building/street security camera) to record how User type the password during authentication on PasswordWand.
Or just watch on device display.

We may try prevent catching the master password by someone else technically:
* hide display behind small walls on case
* (not enough good idea) show on display stars instead of already entered symbols
* (bad idea): make password time/day/etc. dependent - one on Monday, another on Tue and so on. It'll be expired in a day.
* Don't show password on display at all, e.g.
  * (?too complex?) allow to attach bluetooth headset and pronounce currently typing symbol. Therefore intruder will be not able to get know master password by look on display.
  * (not enough good idea) Hide typed part by default, show by demand but print symbols rotated, even maybe each symbol should be rotated independently... I guess customer will be able to recognize what password has been already entered but outside human look will break intruder brain. At the same time if intruder will make a photo then he'll have enough time to decode password symbols.

But hiding display is not enough because buttons and rotate encoder manipulations are visible.

At the same time we might do recorded button/encoder actions sequence useless. Let's force User to enter master password not from 1st to last symbol but randomize process of entering first N (? 5 ?) symbols, e.g. 3th, then 1st, then 5th, then 2nd, then 4th, then - the rest tail.

In such case Intruder will have to guess first N symbols and spend all 10 fail login tries.

#### Intruder detected buttons/encoder actions by sound

Maybe it's a fantastic case but I guess possible if different button clicks can be recognized.

We may try to use sensor buttons to avoid click sound.

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

Cryptocell-310 has NO Key Management Unit so we have to store hash and salt in internal NVM flash memory.

Let's change master password verify procedure:
* Receive password from user
* Salt it
* Encrypt some random string using AES256 in ECB mode
  * Reason: ECB mode does not use IV and output for the same input text is stable for each call
* Ask Cryptocell encrypt the same string using key from TrustZone and AES256 in ECB mode
* Compare result

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

#### Option 4. Device a'la broken

Use case: auth using special password and ...

... device starts to raise error on try send username/password to PC: some fake error about lost link, keyboard initialization error and etc.

... device will show list of accounts (how???) but without ability to unhide passwords or activate CLI.
  * Reason: to avoid show password via any possible outputs

#### Option 5. Implement all variants

You will be able to follow scenarios similar next:
1. say wrong password, lost one login try and some health, say password to open fake storage.
2. say wrong password, lost one login try and some health, say password which perform factory reset.
3. say password which perform factory reset. Possible consequences are on you.
4. say password which lock device for "lock period"

Possibly it's reasonable to add configuration option and allow User to choose preferable behavior scenario.

TBD
