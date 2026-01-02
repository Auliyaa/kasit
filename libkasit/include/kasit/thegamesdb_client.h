#pragma once

#include <kasit/game.h>

#include <list>

namespace kasit
{

struct thegamesdb_clt_p;

class thegamesdb_clt_t
{
public:
  thegamesdb_clt_t(const std::string& api_key);
  virtual ~thegamesdb_clt_t();

  std::string api_key() const;
};

} // ::kasit

