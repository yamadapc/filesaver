macro(run_conan)
# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(
    STATUS
      "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
       "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

message("-- Including conan.cmake")
include(${CMAKE_BINARY_DIR}/conan.cmake)

message("-- Running conan install")
conan_add_remote(NAME conancenter URL
                 https://center.conan.io)

set(CONAN_EXTRA_OPTIONS
  boost:without_locale=True
  boost:without_stacktrace=True
)

conan_cmake_run(
  ARCH
  "${CONAN_ARCH}"
  REQUIRES
  ${CONAN_REQUIRES}
  OPTIONS
  ${CONAN_EXTRA_OPTIONS}
  BASIC_SETUP
  CMAKE_TARGETS
  BUILD
  missing
)
endmacro()
