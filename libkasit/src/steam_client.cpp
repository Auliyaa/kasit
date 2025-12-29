#include <kasit/steam_client.h>

#include <crow.h>
#include <iostream>

#include <regex>
#include <numeric>

// port used locally to fetch steamid64 from login api
constexpr const uint16_t HTTP_CALLBACK_PORT{16888};

namespace kasit
{
struct steam_clt_p
{
  std::string api_key{""};
  uint64_t id{std::numeric_limits<decltype(id)>::max()};

  // open steam login in a web browser
  void open_steam_login()
  {
    std::stringstream login_cmd;
#ifdef _WIN32
    login_cmd << "start";
#elif __APPLE__
    login_cmd << "open";
#else
    login_cmd << "/usr/bin/xdg-open";
#endif

    login_cmd << " "
              << "\'"
              << "https://steamcommunity.com/openid/login?"
              << "openid.ns=http://specs.openid.net/auth/2.0&"
              << "openid.mode=checkid_setup&"
              << "openid.return_to=http://localhost:" << HTTP_CALLBACK_PORT << "/auth/steam&"
              << "openid.realm=http://localhost:" << HTTP_CALLBACK_PORT << "&"
              << "openid.identity=http://specs.openid.net/auth/2.0/identifier_select&"
              << "openid.claimed_id=http://specs.openid.net/auth/2.0/identifier_select"
              << "\'";

    const auto login_cmd_s = login_cmd.str();
  #ifndef NDEBUG
    std::cout << login_cmd_s << std::endl;
  #endif
    system(login_cmd_s.c_str());
  }

  static uint64_t extract_steam_id(const std::string& claimed_id_url)
  {
    std::regex re(R"(https://steamcommunity\.com/openid/id/(\d+))");
    std::smatch match;
    if (!std::regex_match(claimed_id_url, match, re)) return std::numeric_limits<uint64_t>::max();
    return std::stoull(match[1]);
  }
};
}

using namespace kasit;

steam_clt_t::steam_clt_t(const std::string& api_key)
  : _p{new steam_clt_p}
{
  _p->api_key = api_key;
}

steam_clt_t::steam_clt_t(const std::string& api_key, uint64_t id)
  : _p{new steam_clt_p}
{
  _p->api_key = api_key;
  _p->id = id;
}

steam_clt_t::~steam_clt_t()
{
}

std::string steam_clt_t::api_key() const
{
  return _p->api_key;
}

uint64_t steam_clt_t::id() const
{
  return _p->id;
}

void steam_clt_t::login()
{
  // start a callback server on localhost
  // this will receive the steamid from the browser once steam is authenticated
  std::promise<std::string> claimed_id;
  auto claimed_id_future = claimed_id.get_future();

  crow::SimpleApp app;
  CROW_ROUTE(app, "/")([]()
  {
    return crow::response{200};
  });
  CROW_ROUTE(app, "/auth/steam")([&claimed_id](const crow::request& req)
  {
    claimed_id.set_value(req.url_params.get("openid.claimed_id"));
    return crow::response(
      200,
      "Steam login successful. You may now return to the application."
    );
  });

  std::thread app_thread([&app,&claimed_id]()
  {
    try
    {
      // run crow app in a separate thread (will be stopped from current thread)
      app.bindaddr("127.0.0.1").port(HTTP_CALLBACK_PORT).run();
    } catch(const std::exception& x)
    {
      claimed_id.set_value("");
    }
  });

  _p->open_steam_login();
  claimed_id_future.wait(); // TODO: wait for + timeout
  const auto claimed_id_value = claimed_id_future.get();
  // claimed id fetched (or error): stop the crow app
  app.stop();
  app_thread.join();

  _p->id = _p->extract_steam_id(claimed_id_value);
  if (_p->id == std::numeric_limits<decltype(_p->id)>::max())
  {
    // TODO: handle error
  }
}