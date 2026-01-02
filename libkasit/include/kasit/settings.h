#pragma once

#include <kasit/game.h>

namespace kasit
{

struct settings_p;

class settings_t
{
public:
  settings_t();
  virtual ~settings_t();

  bool open(const std::string&);
  void close();

  std::string steam_api_key() const;

  std::string igdb_id() const;
  std::string igdb_secret() const;

private:
  std::unique_ptr<settings_p> _p;
};

} // ::kasit

