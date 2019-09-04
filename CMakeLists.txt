cmake_minimum_required(VERSION 3.14)
project(file_saver)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.68.0 REQUIRED filesystem system)
find_package(rxcpp CONFIG)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/vendor/SQLiteCpp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/vendor/leveldb)
add_subdirectory(tests)

include_directories(
        /usr/local/include
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_LIST_DIR}/vendor/SQLiteCpp/include
        ${CMAKE_CURRENT_LIST_DIR}/vendor/leveldb/include
)

set(
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
        src/services/StorageService.cpp src/services/StorageService.h
        src/workers/StorageWorker.cpp
        src/workers/StorageWorker.h
        src/services/storage/LevelDbStorageService.cpp
        src/services/storage/LevelDbStorageService.h
        src/services/storage/BinaryStorageService.cpp src/services/storage/BinaryStorageService.h)

add_library(
        libfilesaver
        ${SOURCE_FILES}
)

add_executable(
        file_saver
        main.cpp
        src/FileSaver.cpp
        src/FileSaver.h
        src/models/FileEntry.cpp
        src/models/FileEntry.h
        src/data/WorkQueue.h
        src/workers/WorkerManager.cpp
        src/workers/WorkerManager.h
        src/workers/Worker.cpp
        src/workers/Worker.h
        src/services/StorageService.cpp
        src/services/StorageService.h
        src/services/storage/LevelDbStorageService.cpp
        src/services/storage/LevelDbStorageService.h
)

target_link_libraries(
        file_saver
        ${Boost_LIBRARIES}
        SQLiteCpp
        sqlite3
        pthread
        dl
        leveldb
)
