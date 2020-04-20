cmake_minimum_required (VERSION 3.14)

project (file_saver_tests)

set (CMAKE_CXX_STANDARD 17)
set (REPO ~/CLionProjects/.repo)

include (CTest)
include (${CMAKE_CURRENT_LIST_DIR}/../dependencies/conanbuildinfo.cmake)

enable_testing ()

include (${REPO}/DownloadProject/DownloadProject.cmake)
download_project (
  PROJ googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG master
  UPDATE_DISCONNECTED 1
)

add_subdirectory (${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
include_directories (
  /usr/local/include
  ${CMAKE_CURRENT_LIST_DIR}/vendor/leveldb/include
)

set (
  TEST_SOURCE_FILES
  models/FileEntryTest.cpp
  data/WorkQueueTest.cpp
)
list(TRANSFORM TEST_SOURCE_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
message("Test source files:")
foreach(f ${TEST_SOURCE_FILES})
  message("  - ${f}")
endforeach()

include_directories (
  /usr/local/include
)
add_executable (
  filesaver_tests
  gtest.cpp
  ${SOURCE_FILES}
  ${TEST_SOURCE_FILES}
)

target_compile_definitions(
  filesaver_tests
  PRIVATE
  TEST=1
)

target_link_libraries (
  filesaver_tests
  gtest
  gmock
  ${CONAN_LIBS}
  sqlite3
  pthread
  dl
  leveldb
)
