set(
  TEST_SOURCE_FILES
  main.cpp
  data/WorkQueueTest.cpp
  models/FileEntryTest.cpp
)
list(TRANSFORM TEST_SOURCE_FILES PREPEND "${CMAKE_CURRENT_LIST_DIR}/")

add_executable (
  filesaver_tests
  ${TEST_SOURCE_FILES}
)
target_include_directories(
  filesaver_tests
  PRIVATE
  SYSTEM
  ${CMAKE_CURRENT_LIST_DIR}/../src/
)
target_link_libraries (
  filesaver_tests
  PRIVATE
  lfilesaver
  CONAN_PKG::catch2
)
