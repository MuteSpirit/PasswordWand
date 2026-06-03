# PasswordWand

Hardware credential manager is secure replacement of stickers on monitor with login and passwords.

Separate device will store your credentials and type then into login forms.

Keep your creds in secret.

## This project is fork of ...

... https://github.com/seawarrior181/PasswordPump

I'd like to thank Dan Murphy for PasswordPump. It's cool idea and device.
I'm appreciate how many features has been implemented in orignal sketch.

### Forking Reasons

PasswordPump usability is too complex by my opinion.
I'm sure that it's possible to make device more simple and usable.

PasswordPumpII project is exist but it's based on board which price is in 5-7 times higher then for Arduino Pro Micro. Such cool device as hardware credential manager should be as cheaper as possible to be more accecible.

## PasswordPump (original project)

GitHub: https://github.com/seawarrior181/PasswordPump

Author: Dan Murphy aka <seawarrior181>

An ATMega32u4 USB based credentials manager. 
See www.5volts.org for more information.

## **!!! Warning !!!**

Project is under active development.
Please don't use it until first stable release.

# News

* All original code of PasswordPump has been removed from the project. Code base has been developed from scratch.
* AES128 has been replaced with AES256.
* Code related to authentication and encrypted storage has been covered by unit tests (AUnit has been used).
* Unfortunatelly **sketch is too big for Arduino Pro Micro**

# Roadmap

* Try to use NFR52840 Pro Micro instead of Arduino. We'll have 1MB flash instead of 32KB (where 4KB is reserved for bootloader) and 256KB RAM instead of 2.5KB
* Try to replace AES256 with AES128Tiny
* Make test device
* Finish MVP
* Make presentation materials

# PasswordWand Design

See design [DESIGN](DESIGN.md)

I hope that available and clear design can gain the trust of users.

Credential manager is device storing sensitive information and it's used for security purposes.

Customers may trust proprietary solutions due to manufacturer reputation even if device internals is unknown.

Small project should be opened to become acceptable alternative.

# Disclamers

The PasswordWand is not secure from keylogging attacks (https://en.wikipedia.org/wiki/Keystroke_logging). 

Under no circumstances and under no legal theory, whether in tort (including negligence), contract, or otherwise, shall the creator of this device and software be liable to any person for any direct, indirect, special, incidental, or consequential damages of any character arising as a result of the use of the PasswordWand including, without limitation, damages for loss of goodwill, work stoppage, computer failure or malfunction, personal injury, death or any and all other damages or losses. 

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.

This program and device are distributed in the hope that they will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
