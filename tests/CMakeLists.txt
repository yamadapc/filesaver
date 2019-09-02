cmake_minimum_required(VERSION 3.14)

project(file_saver_tests)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.68.0 COMPONENTS filesystem)

set(CMAKE_CXX_STANDARD 17)
set(REPO ~/CLionProjects/.repo)

include(CTest)
enable_testing()

include(${REPO}/DownloadProject/DownloadProject.cmake)
download_project(
        PROJ                googletest
        GIT_REPOSITORY      https://github.com/google/googletest.git
        GIT_TAG             master
        UPDATE_DISCONNECTED 1
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
include_directories(
        /usr/local/include
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_LIST_DIR}/vendor/SQLiteCpp/include
)

set(
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
        ../src/services/StorageService.cpp
        ../src/services/StorageService.h
        services/StorageServiceTest.cpp
)

include_directories(
        /usr/local/include
        ${Boost_INCLUDE_DIRS}
)
add_executable(runUnitTests ${test_SRCS})

target_link_libraries(
        runUnitTests
        gtest
        gmock
        ${Boost_LIBRARIES}
        SQLiteCpp
        sqlite3
        pthread
        dl
)
