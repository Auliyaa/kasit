#include <kasit/steam_client.h>

int main(int argc, char** argv)
{
  // kasit::steam_clt_t steam{""};
  // steam.login();
  kasit::steam_clt_t steam{"0A067931C1A56D9E69C0AF9FBB6D6327", 76561198019154549ULL};
  const auto games = steam.games();

  return 0;
}