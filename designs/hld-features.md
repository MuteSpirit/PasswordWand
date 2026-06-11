# Designs for various features

## Introduction

This document will contain short descriptions of features not related to main use cases. Planned for nearest release and for future.

If some feature design become big it should be moved into separate document.

Proposals for Arduino Pro Micro is kept for history, they will be marked as deprecated.

## MVP Features

## Type-C USB port on board

#### nRF52840 Pro Micro

nRF52840 based boards already has Type-C USB port.

It's need to test how much plug-unplug cycles it can support.

#### (Deprecated) Arduino Pro Micro

Use Arduino Pro Micro with Type-C instead micro USB port.

Reason: modern devices commonly use Type-C and micro USB port is outdated.

### Change master password

#### nRF52840 Pro Micro

It's planned to use internal 1MB memory to keep encrypted credential accounts.

Looks like 128 bytes will be enough for one account.

So for 1000 accounts we need only 128 kB on flash.

It means that it'll be enough size to implement feature following scenario:
* Receive new master password
* Generate new master password salt
* Choose next 128kB + "size for settings" diapason on internal flash
* Write new master password and new salt into that diapason
* For each creds account:
  * decript using old master password and salt
  * encrypt using new master password and new salt
  * write on new place on flash
* Mark 128 kB block with newly encrypted data as active

#### (Deprecated) Arduino Pro Micro

It isn't implementable in in nearest PasswordWand version because
* all credentials are encrypted using old master password
* PasswordWand will not use 2nd external EEPROM chip due to decreasing functionality
* power cut may happen at any time
* primary external EEPROM may be full

It's needed to have 2nd EEPROM to store there credentials encrypted by new master password and make that EEPROM primary.

Or it's needed to implement export credentials into file via serial port. And import too. To be able perform scenario:
* export creds into file
* perform PasswordWand Factory Reset
* set new master password
* import creds from file back to device

### If there are not enough resources on Arduino Pro Micro

What may be tried:
* check what libraries spend the most part of resources and try to replace/rewrite them.
* use another board, e.g.
  * the same as in PasswordPumpII project
  * ESP32-S2 & ESP32-S3: supports full-speed USB-OTG peripheral which is essential for implementing custom HID devices.
    * https://github.com/espressif/arduino-esp32/tree/master/libraries/USB
    * :heavy_plus_sign: It'll be also emulate Bluetooth keyboard and be able to work with mobile devices too
  * nRF52840 Pro Micro
    * [ZMK Firmware](https://zmk.dev/) support and may work as wired/wireless keyboard
    * hardware randimize generator (TRNG)
    * AES encryption
    * SHA256 hashing
    * Bluetooth
    * CPU ARM Cortex-M4F 64MHz, and 1M RAM
    * Low power consumption
    * used for Meshtastick also

## Future Releases Features

### Auto logout on inactivity period expiration

Remember timestamp just after login.

What we treat as activity:
* Send user/password
* Change settings
* CLI usage
* Any button push
* Rotating encoder

Because handling any inputs - buttons, Serial, encoder - theoretically may be done via interrupts then it'll be reasonably to raise timer and rearm it on any activity above happen.

Pattern Observer will be reasonable here to avoid knowledge all code about such specific feature as "auto logout".

On power off this logic becomes outdated so timestamp of last activity and time must be in RAM and are not stored in persistent storage.

On timer expiration authentication must be marked as not passed and master password must be wiped by zeros in RAM.

### Export creds

Possible use case:
1. Plug device to PC
2. Perform device auth
3. Enable CLI
4. Run command "export" ...

### Work as flash drive

For example - store portable versions to work with serial ports - for different Operation Systems.

Use case is next - plug device, it's recornized as USB Mass Storage, mount it, open, copy to PC the application to work with serial ports and run it.

More cool variant - work as boot flash drive.
Attach SD Card reader via SPI.
Format microSD card for several partitions and Syslinux as bootloader.
Plug microSD card into PasswordWand card reader.
Plug device to PC and start it's boot.

Even more cool thing - ability to use PasswordWand as creds manager in parallel and type login/passwords in console, e.g. passwords for TrueCrypt bootloader.

### Work as OTP token

If add special account type which contains not password but OTP secret key.

And on switch to that account in accounts menu OTP password will be shown similar way as in MS Authenticator or similar.

### Each failure try locks device

There is no more factory reset after 10 fail authentications.

Login fail locks the device for some period.

Lock delay increases each time.

Therefore after for example 10 fails device should be locked for seriously long period.

After success auth lock timer will be reset.

(+) Creds will be not lost
(+) Master password broughtforce become unusable due to too much spend time

Possible case:
* 1st auth failure lock device to 5 min
* 2nd auth fail - for 1 hr
* 3rd auth fail - for 2 hr
* 4th - for 1 day
* 5th - for 2 day
* 6th - for 1 week
* 7th - for 2 week
* 8th - for 1 month
* 9th - for 2 month
* etc.
