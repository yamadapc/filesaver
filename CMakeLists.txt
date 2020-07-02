cmake_minimum_required (VERSION 3.14)
project (filesaver CXX)

include (./cmake/StandardProjectSettings.cmake)
include (./cmake/CompilerWarnings.cmake)
include (./cmake/StaticAnalyzers.cmake)

set (CMAKE_CXX_STANDARD 17)
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)
add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)

set (
  CONAN_REQUIRES
  boost/1.71.0@conan/stable
  spdlog/1.6.1
  fmt/6.2.1
  leveldb/1.22
  catch2/2.12.2
  yaml-cpp/0.6.3
  nlohmann_json/3.8.0
  msgpack/3.2.1
  cpp-httplib/0.6.6
)
include (./cmake/Conan.cmake)
run_conan()

execute_process(COMMAND git describe --tags HEAD RESULT_VARIABLE GIT_SHORT_HASH_RESULT OUTPUT_VARIABLE GIT_SHORT_HASH)
string(REGEX REPLACE "\n$" "" GIT_SHORT_HASH "${GIT_SHORT_HASH}")
message("-- GIT_SHORT_HASH=${GIT_SHORT_HASH}")
add_compile_definitions(GIT_SHORT_HASH="${GIT_SHORT_HASH}")

add_subdirectory (src)
add_subdirectory (tests)

add_subdirectory(./src/node_addon)
