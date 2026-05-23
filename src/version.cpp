#include "version.hpp"

void
print_welcome(Print& out)
{
  out.println(F(TITLE " v" VERSION));
  out.println(F("Credential manager"));
  out.println(F(AUTHOR));

  out.print(__DATE__);
  out.print(F(" "));
  out.println(__TIME__);
}
