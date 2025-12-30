#pragma once

#include <kasit/kasit.h>
#include <nlohmann/json.hpp>

namespace kasit
{

enum store_e
{
  unknown,
  steam,
  epic
};

class game_t
{
public:
  // factory function to create game_t instances from different stores
  static game_t from_steam_api(const nlohmann::json&); // from a call to steam's GetOwnedGames api endpoint. see steam_clt_t
  static void from_steam_api(game_t&, const nlohmann::json&);

  // general metadata
  property_rw(std::string, name, "");
  property_rw(store_e, store, store_e::unknown);

  // images
  property_rw(std::string, icon_path, "");
  property_rw(std::string, banner_path, "");
  property_rw(std::string, background_path, "");

  // playtime stats
  property_rw(uint64_t, last_played, 0);
  property_rw(uint64_t, playtime_total, 0);

  // steam-specific metadata
  struct
  {
    property_rw(uint64_t, appid, 0);
  } steam;
};

} // ::kasit
