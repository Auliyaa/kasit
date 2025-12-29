#pragma once

#include <string>
#include <cstdint>
#include <memory>

namespace kasit
{

struct steam_clt_p;

class steam_clt_t
{
public:
  steam_clt_t(const std::string& api_key);
  steam_clt_t(const std::string& api_key, uint64_t id);
  virtual ~steam_clt_t();

  std::string api_key() const;
  uint64_t id() const;

  void login();

private:
  std::unique_ptr<steam_clt_p> _p;
};

} // ::kasit

