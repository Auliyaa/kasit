#include <kasit/kasit.h>

namespace kasit
{

class game_t;

class mtd_provider_t
{
public:
  virtual void fill(kasit::game_t&) const=0;
};

} // ::kasit