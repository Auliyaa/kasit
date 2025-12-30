#include <kasit/game.h>

namespace kasit
{
struct game_p
{
  std::string name;
};
} // ::kasit

using namespace kasit;

game_t::game_t(const std::string& name)
  : _p{new game_p}
{
  _p->name = name;
}

game_t::~game_t()
{
}