#include <kasit/steam_client.h>
#include <kasit/settings.h>

#include <iostream>

int main(int argc, char** argv)
{
  kasit::settings_t settings;
  settings.open("/home/auliyaa/dev/kasit.conf");
  std::cout << settings.steam_api_key() << std::endl;
  std::cout << settings.igdb_id() << std::endl;
  std::cout << settings.igdb_secret() << std::endl;
  return 0;
}