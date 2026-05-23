# Code Style

At current moment it's just absent.
Combination of C and C++ code, migration from one to another, using libraries of different styles did not allow me to choose single one.

When MVP will be ready I'll reformat code for lowCamelCase style.

# STL

Let's avoid it due to significant binary size boost when try to use it.

# Unit Tests

AUnit - https://github.com/bxparks/AUnit
with 
  https://github.com/bxparks/EpoxyDuino

I lost 2 Arduino Pro Micro and 2 pins on another one.

Then less time board will be used then less hardware glitches will happen with prototype built on breadboard with long cables, non-hardly plugged connectors and resistors with long legs which may cross and touch each other when test the device.
