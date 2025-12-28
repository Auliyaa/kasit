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

add_executable(test main.cpp)
install(TARGETS test DESTINATION ${CMAKE_INSTALL_BINDIR} EXPORT ${PROJECT_NAME}-exports)

cmake_setup_default_export(NAMESPACE kasit)