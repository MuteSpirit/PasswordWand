# [Draft, InProgress] HLD. nRF52840 board support

After reaching Arduino ProMicro/Leonardo resource limit for sketch code it has been decided to try use nRF52840 Pro Micro board instead.

Pros:
* :heavy_plus_sign: more resources:
  * in 30 times more internal memory - 1MB vs 32kB
  * in 100 times more RAM - 256kB vs 2.5kB
* :heavy_plus_sign: enough resources to add more features
* :heavy_plus_sign: nRF52840 is used in Meshtastick node projects also, so it's possible to combine Meshtastick and PasswordWand. Maybe even more devices else...

Cons:
* :heavy_minus_sign: need to support new board

I'm afraid that it's not the last time when HW credential manager functionality will be needed on some specific board.

So it's reasonable to define HAL to decrease main code dependency on hardware part.

Let's decide what code stack will be used for HAL.

## Abbreviations

* HLD - High Level Design
* HAL - Hardware Abstraction Layer
* HW - hardware

## Project entities

Let's list what elements are present in planning system:

* Arduino IDE is more preferable development IDE and libs stack for increasing development team
* Dependecy headers/libraries/features:
  * AUnit - unit tests lib
    * indepedent on board
  * Crypto (AES.h, SHA256.h)
  * SimpleCLI (SimpleCLI.h)
    * has not direct dependency on Serial or Wire
    * sketch code is responsible to communicate over Serial
  * arduino-NVM 
    * already has compatible header EEPROM.h
  * EEPROM_SPI_WE (EEPROM_SPI_WE.h) - at first release it may be not used if store creds into internal NVM memory
    * depends on SPI.h
  * Keyboard.h
  * RotaryEncoder (RotaryEncoder.h)
  * Random numbers generation
* Device output:
  * OLED
    * SSD1306Ascii is used for Arduino
  * keyboard HID stack
  * (in future) BLE HID keyboard (to work smoothly with mobile devices)
* Device inputs:
  * 4 buttons
  * Rotary Encoder
  * button in encoder
* Device inout:
  * UART
  * (in future) sensor display
* Board power supply
  * Power supply over USB cable
  * (for future) battery powering support for BLE HID keyboard feature
* Power Cut may happen at any time - USB cable unplug is enough
* Board hardware:
  * TRNG: A True Random Number Generator in nRF52840
  * AES256 HW accelerator in nRF52840

## Option 1. Continue with Arduino IDE

Adafruit has board based on nRF52840 and they shares with community the nRF52 libraries.

Adafruit board manager for nRF52840 in Arduino IDE: https://adafruit.github.io/arduino-board-index/package_adafruit_index.json

There is Arduino libraries for nRF52840:
```
$ ls -1 .../.arduino15/packages/adafruit/hardware/nrf52/1.7.0/libraries/                                                                        
Adafruit_LittleFS
Adafruit_nRFCrypto
Adafruit_TinyUSB_Arduino
BLEAdafruitService
BLEHomekit
Bluefruit52Lib
InternalFileSytem
PDM
RotaryEncoder
Servo
SoftwareSerial
SPI
Wire
```

So nRFCrypto covers encryption and Random Generator, RotaryEncoder covers rotary encoder, SPI covers external EEPROM chips.

tinyUSB covers HID keyboard stack including Bluetooth.

"SSD1306Ascii" lib can work with I2C on AVR only. But "Arduino SSD1306" should work on nRF52840.

Work with buttons only "<Arduino.h>" is needed.

"Serial" works on nRF52840 according to examples from Internet, so SimpleCLI feature will work also.

Conclusion:
* We continue use Arduino IDE
* But HAL implementation will be needed 

## Option 2. Nordic Semiconductor SDK

TBD

Uses [RTOS Zephyr](https://www.zephyrproject.org/).

* :heavy_minus_sign: increase development complexity
* :heavy_minus_sign: separate development tools and environment, Arduino IDE cannot be used more.
* :heavy_minus_sign: Some parts maybe proprietary and closed
* :heavy_plus_sign: better HW abstraction (including Devicetree and KConfig)
* :heavy_plus_sign: unified threads support 

BLE, SPI, I2C support must be investigated!


## Option 3. ZMK

TBD

https://github.com/zmkfirmware/zmk

Also uses [RTOS Zephyr](https://www.zephyrproject.org/).

Zephyr supports nRF52 based boards (official, not clones), e.g. Adafuit https://docs.zephyrproject.org/latest/boards/adafruit/itsybitsy/doc/index.html

## Option 4. The same as Meshtastick use

TBD

## Option ...

TBD
