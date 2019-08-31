cmake_minimum_required(VERSION 3.14)
project(file_saver)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.68.0 REQUIRED filesystem system)

find_package(rxcpp CONFIG)

set(CMAKE_CXX_STANDARD 17)

#add_subdirectory(tests)

include_directories(
        /usr/local/include
        ${Boost_INCLUDE_DIRS}
)

set(
        SOURCE_FILES
        src/services/FileSizeService.cpp
        src/services/FileSizeService.h
        src/FileSaver.cpp
        src/FileSaver.h
        src/workers/WorkerManager.cpp
        src/workers/WorkerManager.h
        src/data/WorkQueue.cpp
        src/data/WorkQueue.h
        src/workers/Worker.cpp
        src/workers/Worker.h
)

add_library(
        libfilesaver
        ${SOURCE_FILES}
)

add_executable(
        file_saver
        main.cpp
        src/FileSaver.cpp
        src/FileSaver.h
        src/services/FileSizeService.cpp
        src/services/FileSizeService.h
        src/data/WorkQueue.cpp
        src/data/WorkQueue.h
        src/workers/WorkerManager.cpp
        src/workers/WorkerManager.h
        src/workers/Worker.cpp
        src/workers/Worker.h
)
target_link_libraries(file_saver ${Boost_LIBRARIES})
