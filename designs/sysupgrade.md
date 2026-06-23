# System Upgrade

## Introduction

I guess it's already generally accepted fact that any software contains bugs. And one of ways to fix that is applying patches, installing new releases, etc. If shortly - use latest and greatest device firmware version.

Theoretically it's possible to make everything good even in first release but even in such case should be a way to receive new releases with new features.

There is a try to design PasswordWand upgrades at this document.

## Hardware revisions

Let's impress that User starts use PasswordWand hardware revision X. It contains some amount of components - buttons, display and etc.

And after some time new hardware revision Y will be released. What options do we have?

If device is DYI then User I guess will be able to change schematics accordingly to new specification.

But if User has no needed competences then he may continue use revision X or buy/receive revision Y and migrate to it somehow.

Another possible case for forcing migrate to another device (even the same hardware version) - broken device physically - crashed display/button(s)/USB-port/etc. 

Contradiction:
* From one side we'd like to make storage with encrypted credential accounts non extractable from device ...
  * Reason: intruder must not get access to creds
* ... but from another side we'd like to have ability of migration/moving credential accounts from one device to another one.

TBD

## Firmware revisions

### Contradiction 1

* From one side we'd like to avoid upgrade sketch to something else ...
  * Reason: uploading sketch by Intruder is one of variants to read internal memory containing sensitive data
* ... but from another side firmware components (bootloader, SoftwareDevice, sketch) should be upgradable
  * Reason: any code contains erros/vulterabilities and updating to new version with fixes is alone way to continue be in safe.

The most straightforward way is use Secure Boot (start read from [this thread](https://forum.seeedstudio.com/t/firmware-protection-nrf52840/291651/4)).

As a result - only signed images will be burned.

### Contradiction 2

* We are going to upgrade firmware parts ...
* ... but new versions might contain errors and brick the device

Let's use "active/commit bank" schema.

By default nRF52840 internal memory contains bootloader, SoftwareDevice and sketch.

We will have two numbers of bootloader, (?) SoftwareDevice (?) and sketch.

On burning new version it'll be written into non active "bank", active bank will be switch to "bank" with new version. And on next boot new version will be started. User'll try to use it and if everything is fine - commit it (== make it main "bank"). Otherwise it'll be enough to reboot device to rollback to previously used firmware.

There will be next on internal NVM:
```
bootloader A
bootloader B
bootloader settings
SoftwareDevice A
SoftwareDevice B
sketch A
sketch B
```

#### Contradiction 2

* We're going to allow birn only signed firmwares ...
* ... but User SHOULD NOT trust official releases to avoid "Attack On Supply Chain" and SHOULD build new firwares itself ...
* ... but Developer MUST NOT provide private key of digital sign to anyone

If PasswordWand is DYI then nRF52840 Pro Micro has non secure bootloader and User can burn anything to it.

If PasswordWand manufactured then maybe it's reasonable to burn nothing on it.

Extention of this case is specialized sketch which will only hint User to burn needed parts and do nothing more.
