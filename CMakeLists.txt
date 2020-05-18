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
  spdlog/1.5.0
  fmt/6.1.2
  leveldb/1.22
  catch2/2.12.1
)
include (./cmake/Conan.cmake)
run_conan()

add_subdirectory (src)
add_subdirectory (tests)

add_subdirectory(./src/node_addon)
