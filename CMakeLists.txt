cmake_minimum_required (VERSION 3.14)
project (filesaver)

set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)
find_package (Boost 1.68.0 REQUIRED filesystem system program_options)
find_package (rxcpp CONFIG)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory (${CMAKE_CURRENT_LIST_DIR}/vendor/SQLiteCpp)
add_subdirectory (${CMAKE_CURRENT_LIST_DIR}/vendor/leveldb)
add_subdirectory (tests)

include_directories (
  /usr/local/include
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_LIST_DIR}/vendor/SQLiteCpp/include
  ${CMAKE_CURRENT_LIST_DIR}/vendor/leveldb/include
)

set (
  SOURCE_FILES
  src/models/FileEntry.cpp
  src/models/FileEntry.h
  src/FileSaver.cpp
  src/FileSaver.h
  src/workers/WorkerManager.cpp
  src/workers/WorkerManager.h
  src/data/WorkQueue.h
  src/workers/Worker.cpp
  src/workers/Worker.h
  src/services/storage/StorageService.h
  src/services/storage/LevelDbStorageService.cpp
  src/services/storage/LevelDbStorageService.h
  src/command-line/CommandLineApp.cpp
  src/command-line/CommandLineApp.h
  src/services/storage/SQLiteStorageService.cpp
  src/services/storage/SQLiteStorageService.h
  src/models/FileSizePair.cpp src/models/FileSizePair.h)

add_library (
  libfilesaver
  ${SOURCE_FILES}
)

add_executable (
  filesaver
  main.cpp
  ${SOURCE_FILES}
)

find_library(CORE_FOUNDATION CoreFoundation)
target_link_libraries (
  filesaver
  ${Boost_LIBRARIES}
  SQLiteCpp
  sqlite3
  pthread
  dl
  leveldb
  ${CORE_FOUNDATION}
)
