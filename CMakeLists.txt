cmake_minimum_required (VERSION 3.14)
project (filesaver CXX)
set (CMAKE_CXX_STANDARD 17)

include (./cmake/StandardProjectSettings.cmake)
include (./cmake/CompilerWarnings.cmake)
include (./cmake/StaticAnalyzers.cmake)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)
add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)

file (READ ./dependencies.txt DEPENDENCIES)
message("-- DEPENDENCIES=${DEPENDENCIES}")
set (
  CONAN_REQUIRES
  ${DEPENDENCIES}
)
include (./cmake/Conan.cmake)
run_conan()

execute_process(COMMAND git describe --tags HEAD RESULT_VARIABLE GIT_SHORT_HASH_RESULT OUTPUT_VARIABLE GIT_SHORT_HASH)
string(REGEX REPLACE "\n$" "" GIT_SHORT_HASH "${GIT_SHORT_HASH}")
message("-- GIT_SHORT_HASH=${GIT_SHORT_HASH}")
add_compile_definitions(GIT_SHORT_HASH="${GIT_SHORT_HASH}")

add_subdirectory (src)
add_subdirectory (tests)
