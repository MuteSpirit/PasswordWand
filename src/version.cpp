#include "version.hpp"

void
print_welcome(Print& out)
{
  out.println(F(TITLE " v" VERSION));
  out.println(F(AUTHOR));
  out.println(__DATE__);
}
