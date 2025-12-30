#pragma once

#include <string>
#include <cstdint>
#include <memory>

namespace kasit
{
  void init();
}

#define property_ro(type, name, defval)\
private:\
  type _##name = defval;\
public:\
 const type& name() const { return _##name; }

#define property_rw(type, name, defval)\
  property_ro(type, name, defval);\
  auto& set_##name(const type& v) { _##name = v; return *this; }
