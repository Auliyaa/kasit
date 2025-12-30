#include <kasit/game.h>

using namespace kasit;

void game_t::from_steam_api(game_t& res, const nlohmann::json& j)
{
  // {
  //   "appid": 3405340,
  //   "name": "Megabonk",
  //   "playtime_forever": 1781,
  //   "img_icon_url": "8e0ff36cdb1076d69347a2796c7ef5ee18b2fee8",
  //   "has_community_visible_stats": true,
  //   "playtime_windows_forever": 1781,
  //   "playtime_mac_forever": 0,
  //   "playtime_linux_forever": 0,
  //   "playtime_deck_forever": 0,
  //   "rtime_last_played": 1762709762,
  //   "playtime_disconnected": 18
  // },
  res.set_name(j["name"]);
  res.set_store(store_e::steam);
  res.set_playtime_total(j["playtime_forever"]);
  res.set_last_played(j["rtime_last_played"]);
  res.steam.set_appid(j["appid"]);
}

game_t game_t::from_steam_api(const nlohmann::json& j)
{
  game_t res;
  from_steam_api(res, j);
  return std::move(res);
}