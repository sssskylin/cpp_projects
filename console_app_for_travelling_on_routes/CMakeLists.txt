cmake_minimum_required(VERSION 3.5.0)
project(labwork6)

set(CMAKE_CXX_STANDARD 20)


include(FetchContent)

FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
)

FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(json)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui.git
  GIT_TAG main
)
FetchContent_MakeAvailable(ftxui)

set(CPR_BUILD_TESTS OFF CACHE BOOL "Disable CPR tests" FORCE)

add_subdirectory(bin)
add_subdirectory(lib)

