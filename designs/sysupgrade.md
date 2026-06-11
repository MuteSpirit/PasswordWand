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

By default nRF52840 internal memory may contain bootloader, SoftwareDevice, sketch, sketch settings, encrypted storage.

In general, new fixes/releases are possible for first three of them.

Contradiction:
* From one side we'd like to avoid upgrade sketch to something else ...
  * Reason: uploading own sketch is one of variants to read internal memory containing sensitive data
* ... but from another side firmware components should be upgradable
  * Reason: use version with fixed all known vulterabilities

TBD
