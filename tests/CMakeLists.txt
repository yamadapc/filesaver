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
)

set(
        test_SRCS
        gtest.cpp
        FileSizeServiceTest.cpp
        ../services/FileSizeService.cpp
        ../services/FileSizeService.h
        ../services/FileSaver.cpp ../services/FileSaver.h ../services/WorkerManager.cpp ../services/WorkerManager.h)

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
)
