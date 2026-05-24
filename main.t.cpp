#include <AUnitVerbose.h>

// #include "src/encrypt.t.cpp"
// #include "src/ext_storage.t.cpp"

void
setup()
{
  Serial.setLineModeUnix(); // use Unix line end instead of DOS
}

void
loop()
{
  aunit::TestRunner::run();
}
