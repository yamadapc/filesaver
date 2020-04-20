# Project set-up
cmake_minimum_required (VERSION 3.14)
project (filesaver)

find_package (rxcpp CONFIG)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -fvisibility-inlines-hidden")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")

# Conan set-up
include (${CMAKE_CURRENT_LIST_DIR}/dependencies/conanbuildinfo.cmake)
conan_basic_setup ()

add_subdirectory (${CMAKE_CURRENT_LIST_DIR}/vendor/leveldb)

include_directories (
  /usr/local/include
  ${CMAKE_CURRENT_LIST_DIR}/vendor/leveldb/include
)

# Source files
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
  src/models/FileSizePair.cpp
  src/models/FileSizePair.h
  src/models/Settings.cpp
  src/models/Settings.h
  src/command-line/logger/StatusPrinter.cpp
  src/command-line/logger/StatusPrinter.h
  src/command-line/logger/StatusDescr.cpp
  src/command-line/logger/StatusDescr.h
)
list(TRANSFORM SOURCE_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
message("Source files:")
foreach(f ${SOURCE_FILES})
  message("  - ${f}")
endforeach()

# Tests set-up
add_subdirectory (tests)

# Library/executable set-up
add_library (
  lfilesaver
  ${SOURCE_FILES}
)
add_executable (
  filesaver
  main.cpp
  ${SOURCE_FILES}
)

# Warnings and linker options
string (REPLACE "-Werror" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
target_compile_options (lfilesaver PRIVATE -Wall -Wextra -pedantic)
target_compile_options (filesaver PRIVATE -Wall -Wextra -pedantic)

find_library (CORE_FOUNDATION CoreFoundation)
target_link_libraries (
  filesaver
  ${CONAN_LIBS}
  pthread
  dl
  leveldb
  ${CORE_FOUNDATION}
)
