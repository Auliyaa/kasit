#
# dependencies
#
find_package(Crow CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(restclient-cpp CONFIG REQUIRED)

#
# libkasit
#
add_library(kasit
  src/steam_client.cpp
)

target_include_directories(kasit PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(kasit
  PRIVATE
    Crow::Crow
    asio::asio
    nlohmann_json::nlohmann_json
    restclient-cpp
)

install(TARGETS kasit
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  EXPORT ${PROJECT_NAME}-exports
)

install(DIRECTORY include/ DESTINATION include)