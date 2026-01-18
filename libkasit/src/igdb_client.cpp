#include <kasit/igdb_client.h>
#include <kasit/game.h>
#include <kasit/log.h>
#include <kasit/lru_cache.h>

#include <restclient-cpp/restclient.h>
#include <restclient-cpp/connection.h>
#include <iostream>

#include <nlohmann/json.hpp>

namespace kasit
{

struct igdb_clt_p
{
  struct
  {
    std::string client_id;
    std::string access_token;
    uint64_t expires_in;
    std::string token_type;
  } oauth;
  
  RestClient::Connection cx{"https://api.igdb.com"};
  std::string last_error;

  lru_cache_t<std::string, std::string> requests_cache{5000}; // TODO: add a setter

  nlohmann::json request(const std::string& endpoint, const std::map<std::string, std::string>& params, bool cache=true)
  {
    if (cx.GetHeaders().empty())
    {
      cx.AppendHeader("Client-ID", oauth.client_id);
      cx.AppendHeader("Authorization", "Bearer " + oauth.access_token);
      cx.AppendHeader("Content-Type", "text/plain");
      cx.AppendHeader("Accept", "application/json");
    }

    std::string body;
    for (const auto& kv : params)
    {
      body += kv.first;
      body += " ";
      body += kv.second;
      body += ";";
    }

    KASIT_LOG_DEBUG(">> " << body);

    std::string d;
    if (requests_cache.get(body, d))
    { // cache hit: do not send a request
      KASIT_LOG_DEBUG(">> http cache hit");
      return nlohmann::json::parse(d);
    }
    else
    { // cache miss: send a request and cache it if successful
      KASIT_LOG_DEBUG(">> http cache miss");
      const auto rsp = cx.post("/v4/" + endpoint, body);
      if (rsp.code != 200)
      { // error: do not fill cache
        KASIT_LOG_DEBUG("!! error code #" << rsp.code << ": " << rsp.body);
        last_error = "IGDB query failed: code #" + std::to_string(rsp.code);
        return nlohmann::json{};
      }
      KASIT_LOG_DEBUG(">> code #" << rsp.code);
      requests_cache.put(body, rsp.body);
      return nlohmann::json::parse(rsp.body);
    }
  }

  nlohmann::json games(const std::string& search)
  {
    static constexpr const char* FIELDS = 
    "alternative_names,"
    "artworks,"
    "category,"
    "cover,"
    "dlcs,"
    "first_release_date,"
    "franchise,"
    "game_engines,"
    "game_modes,"
    "game_status,"
    "game_type,"
    "genres,"
    "multiplayer_modes,"
    "name,"
    "platforms,"
    "rating,"
    "rating_count,"
    "remakes,"
    "remasters,"
    "storyline,"
    "summary,"
    "tags,"
    "themes,"
    "url,"
    "videos";

    const auto res = request(
      "games",
      {
        {"fields", FIELDS},
        {"search", "\"" + search + "\""},
        {"limit", "1"}
      }
    );

    if (res.empty())
    {
      last_error = "empty response";
      return {};
    }

    return res.front();
  }

  nlohmann::json alternative_names(uint64_t id)
  {
    const auto rsp = request(
      "alternative_names",
      {
        {"fields", "checksum,comment,game,name"},
        {"where", "id='" + std::to_string(id) + "'"},
      }
    );
    if (rsp.empty()) return {};
    return rsp.front();
  }
};

} // ::kasit

using namespace kasit;

igdb_clt_t::igdb_clt_t()
  : _p{new igdb_clt_p}
{
}

igdb_clt_t::~igdb_clt_t()
{
}

void igdb_clt_t::login(const std::string& client_id, const std::string& client_secret)
{
  _p->oauth.client_id = client_id;
  _p->oauth.access_token.clear();
  _p->oauth.token_type.clear();

  const std::string url = "https://id.twitch.tv/oauth2/token?grant_type=client_credentials&client_id=" + client_id + "&client_secret=" + client_secret;
  const auto rsp = RestClient::post(url,"application/x-www-form-urlencoded","");
  if (rsp.code != 200) throw std::invalid_argument{"login to igdb failed with code: " + std::to_string(rsp.code)};
  const auto j = nlohmann::json::parse(rsp.body);
  _p->oauth.access_token = j["access_token"];
  _p->oauth.expires_in = j["expires_in"];
  _p->oauth.token_type = j["token_type"];
}

bool igdb_clt_t::connected() const
{
  return !_p->oauth.access_token.empty();
}

std::string igdb_clt_t::last_error() const
{
  return _p->last_error;
}

bool igdb_clt_t::fill(game_t& out) const
{
  auto j = _p->games(out.name());

  // resolve: alternative_names
  if (j.count("alternative_names") != 0)
  {
    std::vector<nlohmann::json> alternative_names_resolved;
    for (auto alternative_name_id : j["alternative_names"])
    {
      const auto alternative_name_resolved = _p->alternative_names(alternative_name_id);
      if (!alternative_name_resolved.empty())
      {
        alternative_names_resolved.emplace_back(alternative_name_resolved);
      }
    }
    j["alternative_names_resolved"] = alternative_names_resolved;
  }

  std::cout << j.dump() << std::endl;

  //  [
  //    {
  //      "id": 125642,
  //      "alternative_names": [
  //        160651,
  //        164652,
  //        178921
  //      ],
  //      "artworks": [
  //        149700,
  //        149701,
  //        149702
  //      ],
  //      "cover": 386712,
  //      "first_release_date": 1733443200,
  //      "game_engines": [
  //        149
  //      ],
  //      "game_modes": [
  //        1,
  //        2,
  //        3,
  //        5
  //      ],
  //      "genres": [
  //        12,
  //        25,
  //        31
  //      ],
  //      "multiplayer_modes": [
  //        31715
  //      ],
  //      "name": "Path of Exile 2",
  //      "platforms": [
  //        169,
  //        6,
  //        167,
  //        14
  //      ],
  //      "rating": 82.05805231613058,
  //      "rating_count": 169,
  //      "storyline": "20 years have passed.\n\nSince the death of Kitava, society has been slowly rebuilding. Ambitious men rise to fill the void and their thirst for power is once again corrupting Wraeclast.",
  //      "summary": "Path of Exile 2 is a next generation Action RPG created by Grinding Gear Games. Set years after the original Path of Exile, you will return to the dark world of Wraeclast and seek to end the corruption that is spreading. Path of Exile 2 is a free-to-play online multiplayer game with co-op for up to six players.",
  //      "tags": [
  //        1,
  //        17,
  //        268435468,
  //        268435481,
  //        268435487,
  //        536895132
  //      ],
  //      "themes": [
  //        1,
  //        17
  //      ],
  //      "url": "https://www.igdb.com/games/path-of-exile-2",
  //      "videos": [
  //        31207,
  //        31217,
  //        78724,
  //        78725,
  //        91350,
  //        91762,
  //        102511,
  //        114732,
  //        123445
  //      ],
  //      "game_status": 4,
  //      "game_type": 0
  //    }
  //  ]

  return true;
}