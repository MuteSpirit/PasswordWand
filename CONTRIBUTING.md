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

Naming:
```
src/<module>.hpp
src/<module>.cpp
src/<module>.t.cpp
```
where `<module>` is name of file/feature/module

Reasons:
1. Unit tests should be as closer as possible to the code. So tests are located at folder with sources and use middle extension `.t.` to be sorted closer to tested code but not become too long file name.
2. But not in the same files to not mix working and testing code. And maybe have ability to write tests on several languages, test frameworks for the same module.

# Commit signing

To try avoid troubles from "Software supply chain attack" let's sign each commit.

# Commit message

Add "Signed-off-by" trailer in commit message.
