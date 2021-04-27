message("-- Setting-up lfilesaver")
set (
  SOURCE_FILES
  ${SOURCE_FILES}
  FileSaver.cpp
  FileSaver.h
  data/WorkQueue.h
  models/FileEntry.cpp
  models/FileEntry.h
  models/FileSizePair.cpp
  models/FileSizePair.h
  server/Server.cpp
  server/Server.h
  services/filesize/FileSizeService.cpp
  services/filesize/FileSizeService.h
  services/filesize/InMemoryFileEntryStore.cpp
  services/filesize/InMemoryFileEntryStore.h
  services/settings/SettingsService.cpp
  services/settings/SettingsService.h
  services/storage/LevelDbStorageService.cpp
  services/storage/LevelDbStorageService.h
  services/storage/StorageService.h
  simple_timer/SimpleTimer.cpp
  simple_timer/SimpleTimer.h
  utils/Utils.cpp
  utils/Utils.h
  workers/Worker.cpp
  workers/Worker.h
  workers/WorkerManager.cpp
  workers/WorkerManager.h
  services/BackgroundQueueWorker.h
  factory/FileSaverFactory.cpp
  factory/FileSaverFactory.h
  services/BackgroundWorker.h
  services/storage/StorageWorker.cpp
  services/storage/StorageWorker.h
  services/filesize/InMemoryFileSizeService.cpp
  services/filesize/InMemoryFileSizeService.h
  services/filesize/AggregationWorker.cpp
  services/filesize/AggregationWorker.h
  statsd/StatsdClient.cpp
  statsd/StatsdClient.h
  statsd/UDPSender.cpp
  statsd/UDPSender.h
  services/stats/InMemoryMetricsReporter.cpp
  services/stats/InMemoryMetricsReporter.h
  services/paths/RootPathProvider.cpp
  services/paths/RootPathProvider.h
  services/stats/ThroughputTracker.cpp
  services/stats/ThroughputTracker.h
  server/util/DefaultStatsProvider.cpp
  server/util/DefaultStatsProvider.h
  server/util/StatsProvider.h
  server/util/Stats.cpp
  server/util/Stats.h
  services/category/FileCategoryService.cpp
  services/category/FileCategoryService.h
  services/category/FileCategoryWorker.cpp
  services/category/FileCategoryWorker.h
  services/category/PathMatcher.cpp
  services/category/PathMatcher.h
  services/category/LevelDbFileCategoryStore.cpp
  services/category/LevelDbFileCategoryStore.h
  services/storage/LevelDbFactory.cpp
  services/storage/LevelDbFactory.h
)
list(TRANSFORM SOURCE_FILES PREPEND "${CMAKE_CURRENT_LIST_DIR}/")
add_library (
  lfilesaver
  STATIC
  ${SOURCE_FILES}
)
target_compile_options (lfilesaver INTERFACE ${PROJECT_WARNINGS})

set(
  lfilesaver_CONAN_LIBS
  CONAN_PKG::boost
  CONAN_PKG::spdlog
  CONAN_PKG::fmt
  CONAN_PKG::leveldb
  CONAN_PKG::yaml-cpp
  CONAN_PKG::fruit
  CONAN_PKG::nlohmann_json
  CONAN_PKG::cpp-httplib
)
message("-- CONAN_PKG=${CONAN_PKG}")
message("-- lfilesaver_CONAN_LIBS=${lfilesaver_CONAN_LIBS}")
message("-- CONAN_LIB_DIRS=${CONAN_LIB_DIRS}")

target_link_directories(
  lfilesaver
  PUBLIC
  ${CONAN_LIB_DIRS}
)
target_link_libraries (
  lfilesaver
  PUBLIC
  project_warnings
  project_options
  pthread
  dl
  ${lfilesaver_CONAN_LIBS}
)
