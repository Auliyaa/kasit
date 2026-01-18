#include <kasit/kasit.h>

namespace kasit
{

class game_t;

class mtd_provider_t
{
public:
  virtual bool fill(kasit::game_t&) const=0;
  virtual std::string last_error() const=0;
};

} // ::kasit