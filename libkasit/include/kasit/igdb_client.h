#pragma once

#include <kasit/metadata_provider.h>

namespace kasit
{

struct igdb_clt_p;

class igdb_clt_t: public mtd_provider_t
{
public:
  igdb_clt_t();
  virtual ~igdb_clt_t();

  void login(const std::string& client_id, const std::string& client_secret);
  bool connected() const;

  virtual bool fill(game_t&) const override;
  virtual std::string last_error() const override;

private:
  std::unique_ptr<igdb_clt_p> _p;
};

} // ::kasit