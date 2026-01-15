#include <kasit/igdb_client.h>
#include <kasit/game.h>

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

void igdb_clt_t::fill(game_t& out) const
{
  static const std::string FIELDS = 
  "fields "
  "name,"
  "first_release_date,"
  "genres.name,"
  "cover.image_id,"
  "artworks.image_id,"
  "screenshots.image_id,"
  "involved_companies.developer,"
  "involved_companies.publisher,"
  "involved_companies.company.name,"
  "multiplayer_modes.maxplayers,"
  "multiplayer_modes.splitscreen,"
  "multiplayer_modes.offlinecoopmin,"
  "multiplayer_modes.offlinecoopmax,"
  "multiplayer_modes.onlinecoopmin,"
  "multiplayer_modes.onlinecoopmax"
  ";";

  const std::string query = 
    FIELDS +
    "search \"" + out.name() + "\";" +
    "limit 1;";

  RestClient::Connection cx{"https://api.igdb.com"};
  cx.AppendHeader("Client-ID", _p->oauth.client_id);
  cx.AppendHeader("Authorization", "Bearer " + _p->oauth.access_token);
  cx.AppendHeader("Content-Type", "text/plain");

  const auto rsp = cx.post(
    "/v4/games",
    query
  );

  if (rsp.code != 200)
  {
    throw std::runtime_error{"IGDB query failed: code #" + std::to_string(rsp.code)};
  }

  std::cout << rsp.body << std::endl;
}