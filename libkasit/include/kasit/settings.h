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

  std::string api_steam() const;

private:
  std::unique_ptr<settings_p> _p;
};

} // ::kasit

