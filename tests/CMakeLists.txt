set(
  TEST_SOURCE_FILES
  # Catch main
  main.cpp
  # Test files
  data/WorkQueueTest.cpp
  models/FileEntryTest.cpp
  models/FileSizePairTest.cpp
  services/settings/SettingsServiceTest.cpp
  services/storage/LevelDbStorageServiceTest.cpp
  simple_timer/SimpleTimerTest.cpp
  workers/WorkerTest.cpp
  test_utils/TestUtils.cpp
  test_utils/TestUtils.h
  services/filesize/InMemoryFileEntryStoreTest.cpp
  services/BackgroundWorkerTest.cpp
  factory/FileSaverFactoryTest.cpp
  test_utils/RandomRootGenerator.cpp
  test_utils/RandomRootGenerator.h
  integration_tests/MockRootPath.cpp
  services/stats/MetricsReporterTest.cpp
  services/storage/LevelDbFactoryTest.cpp
  services/storage/LevelDbFileCategoryStoreTest.cpp
  services/fileutils/CompressionServiceTest.cpp
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
