cmake_minimum_required(VERSION 3.26)

# --------------------------------------
# declare project based on info from vcpkg.json
# --------------------------------------
file(READ "vcpkg.json" VCPKG_JSON)
string(JSON VCPKG_PROJECT_NAME GET "${VCPKG_JSON}" "name")
string(JSON VCPKG_PROJECT_VERSION GET "${VCPKG_JSON}" "version")
project(${VCPKG_PROJECT_NAME} VERSION ${VCPKG_PROJECT_VERSION})

include(cmake/helpers.cmake)

cmake_setup_default_project()

add_subdirectory(libkasit)

add_executable(meh main.cpp)
target_link_libraries(meh PRIVATE kasit)
install(TARGETS meh DESTINATION ${CMAKE_INSTALL_BINDIR})

cmake_setup_default_export(NAMESPACE kasit)