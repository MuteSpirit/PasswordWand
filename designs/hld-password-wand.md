# Use Cases/Features

## Edit credentials via board CLI

### PasswordPump v1.3 Problematic

PasswordPump device has limited capabilities to enter information by human because has only rotary encoder with embed button. Typing long names, logins and password spend a lot of time.

At the same time pushing button on encoder may rotate it and change the input. I made masster password typing error often due to such rotary encoder button side effect.

PassPumpGUI has been added to give ability add accounts from PC to device but usability is not enough smooth: you should to switch between PasswordPump and computer to type on PC and rotate on device.

Also PassPumpGUI limits computers which may be used because it's should be installed together with dependencies. Mobile apps becomes unusable at that case.

If you are going to use your PasswordPump device then my advice to you - build portable version of PassPumpGUI using project like [PyInstaller](pyinstaller.org) for Linux and Windows. Such PassPumpGUI variant with contain Python and dependency libraries embed. Put it on flash drive and keep near PasswordPump.

### Proposal

There is another idea for PasswordWand: "Use device embed Common Line Interface (CLI) to edit credentials via serial connection".

1. User attach PasswordWand to PC
2. User login on PasswordWand and activate CLI
3. User run picocom/etc on PC and open device serial port
4. Type command in console to manage credentials account on PasswordWand device - ls, add, del and so on
5. Logout after job finish. CLI will be deactivated.

![Use case](docs/cli-use-case.png)

* :heavy_minus_sign: you still need an application to work with serial console like picocom or minicom on Linux, TerTerm on Windows and so on.
* :heavy_minus_sign: more code on device, theoretically board resources will be not enough for that feature.
* :heavy_plus_sign: not needed to maintain/develop PassGUI application
* :heavy_plus_sign: less dependency on OS and Python/deps presence on it. Theoretically mobile OSes may be used too.
* :heavy_plus_sign: not needed more to care about PassGUI compatibility with credential manger device. PasswordPump will provide all available edit credential features via CLI.
* :heavy_plus_sign: use case is more natural - you enable CLI on device and then continue change credentials only on PC
* :heavy_plus_sign: we may remove from PasswordWand all menu items related to edit something because that may happen from PC. Sketch size will be decreased.

:bangbang: Need to check on Arduino Pro Micro because CLI development has been done on Arduino Uno board and I don't know the Pro Micro behavior. Arduino Uno has been used due to simple sketch upload procedure then on Pro Micro (ISP programmer, disconnecting I2C display before each sketch upload).
But as far as PassPumpGUI can work then CLI should work also.

## Random generator

PasswordPump reads the voltage from ADC_READ_PIN floating pin to seed the random number generator. That pin should be not ground.

Such way of receiving random values depends on electro magneric fields accross the device.

To increase randomizing we may try next things:
* gather voltage several times and summarize them
* we may multiply and summarize them, e.g. multiple 4 and summarize with 3
* use another source of entrophy, i.e. light. Set photoresistor (and one more current limiting resistor) between another analog pin and ground and receive voltage on that pin

I've tested 3 entrophy sources on Arduino Uno:
1. unplugged pin
2. 10k Om + photoresistor
3. digital temperature sensor

Conclusion: all are similar to each other but "unplugged pin" will be less cost

```
21:05:16.035 -> Temp V=246
21:05:16.035 -> Unplugged pin V=246
21:05:16.234 -> Photodiode V=235
21:05:16.234 -> Temp V=237
21:05:16.234 -> Unplugged pin V=238
21:05:16.433 -> Photodiode V=227
21:05:16.433 -> Temp V=231
21:05:16.433 -> Unplugged pin V=232
21:05:16.632 -> Photodiode V=224
21:05:16.632 -> Temp V=228
21:05:16.632 -> Unplugged pin V=228
21:05:16.831 -> Photodiode V=224
21:05:16.831 -> Temp V=228
21:05:16.831 -> Unplugged pin V=228
21:05:17.063 -> Photodiode V=226
21:05:17.063 -> Temp V=233
21:05:17.063 -> Unplugged pin V=231
21:05:17.263 -> Photodiode V=235
21:05:17.263 -> Temp V=238
21:05:17.263 -> Unplugged pin V=236
21:05:17.462 -> Photodiode V=244
21:05:17.462 -> Temp V=246
21:05:17.462 -> Unplugged pin V=242
21:05:17.661 -> Photodiode V=256
21:05:17.661 -> Temp V=256
21:05:17.661 -> Unplugged pin V=250
21:05:17.860 -> Photodiode V=268
21:05:17.860 -> Temp V=265
21:05:17.860 -> Unplugged pin V=258
21:05:18.059 -> Photodiode V=279
21:05:18.059 -> Temp V=275
21:05:18.059 -> Unplugged pin V=267
21:05:18.258 -> Photodiode V=291
21:05:18.258 -> Temp V=284
```

## Typing username/password on PC with PasswordPump

When credentials have been added via CLI the main PasswordPump responsibility is enter username and password into login form on PC. And it should be done with as less minimal steps as possible.

1. User attach PasswordWand to PC
2. User auth on PasswordWand
3. User see available accounts just after login
3. User find needed account by rotating encoder
4. User use next actions on 4 buttons to fill login form:
  * Send username to PC
  * Send password to PC
  * Push Tab on PC
  * Push Enter on PC
* :heavy_plus_sign: usability is better. User is able to perform all steps on device without switch between PasswordWand and PC mouse/keyboard. Even if website/application login form is complex and several Tabs are required to jump from username edit field to password one
* :heavy_minus_sign: more buttons on device
* :heavy_minus_sign: not tested usability with maximum of added accounts. Device should work fast and stable on "Find account" navigate step.

## Board USB port

Use Arduino Pro Micro with Type-C instead micro USB port.

Reason: modern devices commonly use Type-C and micro USB port is outdated.

## Authentication and related

There are several aspects related to authentication theme:

* Type master password
* Verify master password
* Change master password
* Store master password in device RAM
  * Reason: in case of power cut it must not survive device reboot
* Use master password for decrypt/encrypt credentials
* Auto logout on inactivity period expiration
* Auto logout on power reboot

#### Related system elements

* rotate encoder
* 3-4 buttons
* buttons click loudly on push
* OLED display showing typing process
* master password length limitation
* symbols possible to use for master password (letters, digits, etc.)
* we cannot store master password in plain view in any EEPROM
* device case
* 3-color LED lamp
* internal and external EEPROM
* short and long button press/hold
* hold some button
* push down and pull up some button
* ability to add another input peripheral
* useless case to push more then one button simultaneously
* useless case to hold encoder button and rotate encoder itself
* Serial connection with PC via USB cable

### Master password typing

#### Proposals

Contradiction:
* Password should be as complex as possible or as longer as possible
* There are 94 symbols in summary which may be used for each master password symbol:
  * latin alpha uppercase
  * latin alpha lowercase
  * numeric
  * special symbols, e.g. "/?><,:';|}{][+\_)(\*&^%$#!~=\-@." and so on
  * space
* BUT we have only rotate encoder to choose needed symbol. With straightforward solution we'd make many rotation for each symbol choice.

Related requirement: if User made a mistake he should not start a quest to rotate 8+ symbols from scratch

We should help user to type password as easier as possible without decrease password complexity

![master password typing proposals](docs/master-password-typing.png)

* Rotate encoder scroll current symbol from available symbolic group
* Button "square" will choose/change such group to rotate smaller times
* Button "cross" will switch symbolic group to group with the same symbols but in another register - lowercase or uppercase
* Button "triangle" will erase last symbol (=== Backspace) to decrease amount of login failures due to one mistake in the middle of long password
* Button "circle" will commit password and trigger authentication procedure
* Master password will be visible during typing
* Bigger OLED display will be used to be able to contain status bar with help hint about buttons
* Typing master password will be stored into RAM only and not on EEPROM
  * Reason: avoid 100% compomizing credentials if device will be stolen after partial master password entering

#### Rejected Idea No1

User type not a password but some analog of PIN.

For example on 1st device boot the random sequencies will be generated for each possible digit 0-9, maybe for each digit position of PIN. For example,

* 1st digit of PIN
  * "1" - 8 random symbols
  * "2" - another 8 random symbols
  * ...
  * "9" - another 8 random symbols
* 2nd digit of PIN - the same

When User enter PIN then internally instead of PIN the consequent random sequences will be used instead of PIN digits.

For example PIN contains 4 digits but internally masster password will contain 32 random symbols.

Reject reason: it's too easy for stranger to remember such password and use after device stealling.

#### Ideas for future device revisions

* Use sensor buttons to avoid loud clicks listen by another persons
* Show only last symbol and star ("\*") instead of previous once. Encoder button push will show full password.
* deepen OLED display in the case to decrease chanse that somebody will see on display and remember your master password

1. Use fingerprint sensor and access by fingerprint
  * Requirement 1: fingerprint sample will be stored at internal EEPROM instead of external to avoid situation with physically removing external EEPROM from device and read data including stored fingerprint
  * Requirement 2: internal EEPROM size must be enough to store fingerprint samples
  * Requirement 3: Arduino must be protected from internal EEPROM reading. (?)
  * Note: there is lib Adafruit_Fingerprint library
2. Use touch screen for navigation instead of rotate encoder + buttons
  * :heavy_minus_sign: I think that development will be more complex
  * :heavy_minus_sign: UI/UX must be adapted to touch screen

### Verify master password

On set master password first time:
* Generate salt randomly
* Add salt to password
* Generate hash
* Store at internal EEPROM

On authentication
* Add salt to password
* Generate hash
* Compare with hash stored previously at internal EEPROM
* If the same then auth passed otherwise increase login fail counter
* store login fail counter at internal EEPROM

### Change master password

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

## Encryption

AES128 is used now and encryption and SHA256 for hashing.

:question: why AES256 is not used? Looks like the same encryption library supports AES256 too.

Salt is needed to defense from situation when 
* intruder received external EEPROM with stored encrypted credentials ...
* ... and know master password
Without knowledge about salt he will be not able to decrypt data in suitable time.

Salt must be generated on 1st boot and on factory reset.

Salt must be stored in internal EEPROM.

Salt must be enough strong to make broutforce much harder.

Master password must be stored in RAM only.

Salted Master Password hash must be stored in internal EEPROM.

Salt for each credential account is also needed. It's true that PasswordWand will encrypt them instead of calculation hashes but if not use the unique salt for each account then even encrypted data may show that for more then one site the same login and passwords are used. This project is OpenSource and data storage structure will not be a secret.

So if combine master password with credential salt for decrypt/encrypt the name, username and password then we'll be able to hide information about some login/pass equiality for different services.

Arduino Pro Micro has only 1Kb of internal EEPROM. So we cannot store salt for 254 accounts there. So unique salt will be stored inside each credential account on external EEPROM.
It's more scalable solution because theoretically it's possible to use bigger 25LC512 and use several chips to store enough more credentials. At that case internal EEPROM cannot store all needed salts anyway.

## Internal EEPROM Layout

| Parameter Name | Size, bytes | Purpose |
| --- | --- | --- |
| Reserved | 4 | Reserved for Layout version - EEPROM data store schema version. |
| Master Password Salt | 32 | |
| Master Password Hash | 32 | |
| Master Password Commit Flag | 1 | Set to 0x1 after store both salt and hash |
| Fail Login Count | 1 | Amount of failed device login after last success authentication |

We have to take power cut event into account designing all operations with internal EEPROM. If operation happen with data which size more then byte the additional "commit flag" byte is needed.

## External EEPROM Layout

| Parameter Name | Size, bytes |Purpose |
| --- | --- | --- |
| Reserved | 32 | 1 reserved page |
| Creds Account 0. Commit Flag | 1 | 0x00 or 0xFF for free account place. 0x01 if initialized. Fill it when all other fields will be written |
| Creds Account 0. Name      | 32 | Encrypted (?) |
| Creds Account 0. Username  | 32 | Encrypted |
| Creds Account 0. Password  | 32 | Encrypted |
| Creds Account 0. Salt      | 16 | Plain text |
| ... | ... | ... |
| Creds Account N. Commit Flag | 1  | 0x00 or 0xFF for free account place. 0x01 if initialized. |
| Creds Account N. Name      | 32 | Encrypted (?) |
| Creds Account N. Username  | 32 | Encrypted |
| Creds Account N. Password  | 32 | Encrypted |
| Creds Account 0. Salt      | 16 | Plain text |
| Reserved | (1 + 32 + 32 + 32 + 16) and aligh to page border -> 128 | Reserved space for future. Maybe it'll be needed to swap accounts on EEPROM |
| Reserved | 32 | 1 reserved page |
| Reserved | 31 | page for reset flag |
| Reset Flag | 1 | Flag that EEPROM initialized or not | 

Max number of possible accounts is ((256kB - 32 - 64 - 32) / 128) = 2047.
But current code assume not more the 254.

PasswordPump v1.3 store credential accounts in double linked list structure on external EEPROM. My opinion that it's a mistake because any unexpected power cut may corrupt that structure:

![Don't use DL List](docs/do-not-use-linked-list.png)

:heavy_plus_sign: avoid troubles on power cut
:heavy_minus_sign: accounts will be shown in mix turn if perform add, del, add operations

We have to take power cut event into account designing all operations with external EEPROM too.

For example add new account should be done like:
* Store name
* Store username
* Store password
* Store salt
* Set account commit flag. In last turn.

External EEPROM reinitialization must be done by wiping all data on EEPROM with zeros to be sure that they are descroyed.

## If there are not enough resources on Arduino Pro Micro

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

## UI

OLED 128x64 will be used.
3-color LED will be not used in first versions. I guess larger display will allow to make something like status bar for indication board activity/progress

### Splash screen

Print on display and into Serial (if available):
* Sketch name
* Sketch version
* "Creds manager"
* Author (?)
* Sketch compilation date (? and time ?)

For example,
```
PasswordWand v0.1
Credential Manager
19 May 2026
Ivan Efimov aka MuteSpirit
```
### Menus 

![UI](docs/screens.png)

# PasswordPump Features to change/remove

init flag as the last 

Not use rotary encoder button.

1 EEPROM chip instead of 2

How many buttons to use.

Plans to switch to 25LC512

Store settings in internal EEPROM

Not store creds style

# Areas to improve

Factory Reset

EEPROM wipe with eraseCompleteEEPROM

Use "Hold" pin and continuous put

Add Bluetooth

Fast navigation between accounts (groups of accounts?)
