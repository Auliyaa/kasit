#include <kasit/steam_client.h>
#include <kasit/settings.h>
#include <kasit/igdb_client.h>

#include <iostream>

int main(int argc, char** argv)
{
  kasit::settings_t settings;
  settings.open("/home/auliyaa/dev/kasit.conf");
  
  kasit::igdb_clt_t igdb;
  igdb.login(settings.igdb_id(), settings.igdb_secret());

  kasit::game_t g;
  g.set_name("Path of Exile 2");
  igdb.fill(g);

  return 0;
}