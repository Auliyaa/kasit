#pragma once

#include <kasit/kasit.h>

namespace kasit
{

enum store_e
{
  steam
};

struct game_p;

class game_t
{
public:
  game_t(const std::string& name);
  virtual ~game_t();

private:
  std::unique_ptr<game_p> _p;
};

} // ::kasit
