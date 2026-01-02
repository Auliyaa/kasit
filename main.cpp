#include <kasit/steam_client.h>
#include <kasit/settings.h>

#include <iostream>

int main(int argc, char** argv)
{
  kasit::settings_t settings;
  settings.open("/home/auliyaa/dev/kasit.conf");
  std::cout << settings.api_steam() << std::endl;
  return 0;
}