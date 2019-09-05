cmake_minimum_required (VERSION 3.14)

project (file_saver_tests)

set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_RUNTIME OFF)
find_package (Boost 1.68.0 COMPONENTS filesystem system program_options)

set (CMAKE_CXX_STANDARD 17)
set (REPO ~/CLionProjects/.repo)

include (CTest)
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
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_LIST_DIR}/vendor/SQLiteCpp/include
  ${CMAKE_CURRENT_LIST_DIR}/vendor/leveldb/include
)

set (
  test_SRCS
  gtest.cpp
  ../src/models/FileEntry.cpp
  ../src/models/FileEntry.h
  ../src/FileSaver.cpp
  ../src/FileSaver.h
  ../src/workers/WorkerManager.cpp
  ../src/workers/WorkerManager.h
  ../src/data/WorkQueue.h
  ../src/workers/Worker.cpp
  ../src/workers/Worker.h
  models/FileEntryTest.cpp
  data/WorkQueueTest.cpp
  ../src/services/storage/StorageService.h
  services/storage/SQLiteStorageServiceTest.cpp
  ../src/services/storage/LevelDbStorageService.cpp
  ../src/services/storage/LevelDbStorageService.h
  ../src/command-line/CommandLineApp.cpp
  ../src/command-line/CommandLineApp.h
  ../src/services/storage/SQLiteStorageService.cpp
  ../src/services/storage/SQLiteStorageService.h
  ../src/models/FileSizePair.cpp ../src/models/FileSizePair.h)

include_directories (
  /usr/local/include
  ${Boost_INCLUDE_DIRS}
)
add_executable (filesaver_tests ${test_SRCS})

target_compile_definitions(
  filesaver_tests
  PRIVATE
  TEST=1
)

target_link_libraries (
  filesaver_tests
  gtest
  gmock
  ${Boost_LIBRARIES}
  SQLiteCpp
  sqlite3
  pthread
  dl
  leveldb
)
