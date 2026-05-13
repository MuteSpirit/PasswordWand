# PasswordWand

Fork of https://github.com/seawarrior181/PasswordPump

When I tried to make PasswordPump device I found that ...
* code can't be compiled at 2026 year 
* committed "\*.hex" files don't want to work after burn to the board. I've broken two Arduino Pro Micro during my tries.
* PassPumpGUI cannot be run with Python3. At the same time Python2 is deprecated.

I know about PasswordPumpII project. But it's based on another board and I still think that such device should be as cheap as possible. 

Roadmap:
* [x] Adapt ./PassPumpGUI_v0_2.py code for Python3 execution with 
* [x] Compile sketch
* [~] Replace PassPumpGUI with embed CLI to avoid need to install any to host PC
* Replace USP connection with Bluetooth one. Keep USB for CLI usage only
* Replace Rotate Encoder with several (3-4) buttons. Encoder is not so accurate and usable by my opinion. And buttons will be easier to hide under case to avoid peeping master password typing.

# POC

Use Arduino Uno for easier sketch uploading.
Test keyboard functionality is not possible but others - CLI, OLED, encoder and EEPROM, - can be developed.

Arduino Uno
OLED SSD1306 128x32

Pins:
* 5V - to OLED VCC, to ...
* GND - to OLED GND, to ...
* A4 - OLED SDA
* A5 - OLED SCK

Libraries:
* SimpleCLI
* GyverOLED
* GyverOLEDMenu

# Original README

## PasswordPump

An ATMega32u4 USB based credentials manager.  See www.5volts.org for more information.

The following libraries need to be installed if you're going to work with the source code:

  - https://rweather.github.io/arduinolibs/index.html - AES and SHA library
  - https://github.com/LennartHennigs/Button2 - Used for the button on the 
    rotary encoder
  - https://github.com/brianlow/Rotary - Used for the rotary encoder
  - https://github.com/arduino-libraries/Keyboard - Used to send characters to 
    the keyboard as if typed by the user
  - https://www.arduino.cc/en/Reference/EEPROM - Used for internal EEprom
  - https://github.com/greiman/SSD1306Ascii/blob/master/examples/AvrI2c128x32/AvrI2c128x32.ino
    for SSD1306 display device

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.

This program and device are distributed in the hope that they will be 
useful, but WITHOUT ANY WARRANTY; without even the implied warranty 
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
