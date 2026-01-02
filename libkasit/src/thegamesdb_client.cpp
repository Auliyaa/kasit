#include <kasit/thegamesdb_client.h.h>

#include <restclient-cpp/restclient.h>

#include <nlohmann/json.hpp>

#include <regex>
#include <numeric>

namespace kasit
{
struct thegamesdb_clt_p
{
  std::string api_key{""};
};
}

using namespace kasit;

thegamesdb_clt_t::thegamesdb_clt_t(const std::string& api_key)
  : _p{new steam_clt_p}
{
  _p->api_key = api_key;
}

thegamesdb_clt_t::~thegamesdb_clt_t()
{
}

std::string thegamesdb_clt_t::api_key() const
{
  return _p->api_key;
}