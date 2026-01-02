#include <kasit/settings.h>

#include <fstream>
#include <inipp.h>

namespace kasit
{
struct settings_p
{
  inipp::Ini<char> ini;

  template<typename T>
  inline T get(const std::string& section, const std::string key, const T& defval={})
  {
    const auto lu = ini.sections.find(section);
    if (lu == ini.sections.end()) return defval;
    const auto lu2 = lu->second.find(key);
    return lu2 == lu->second.end() ? defval : lu2->second;
  }
};
} // ::kasit

using namespace kasit;

settings_t::settings_t()
  : _p{new settings_p}
{
}

settings_t::~settings_t()
{
}

bool settings_t::open(const std::string& p)
{
  std::ifstream is{p};
  if (!is.is_open()) return false;
  _p->ini.parse(is);
  _p->ini.interpolate();
  if (!_p->ini.errors.empty()) return false;
  return true;
}

void settings_t::close()
{
  _p->ini = decltype(_p->ini){};
}

std::string settings_t::steam_api_key() const
{
  return _p->get<std::string>("steam", "api_key");
}

std::string settings_t::igdb_id() const
{
  return _p->get<std::string>("igdb", "id");
}

std::string settings_t::igdb_secret() const
{
  return _p->get<std::string>("igdb", "secret");
}
