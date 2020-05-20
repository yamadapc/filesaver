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
  models/Settings.h
  services/storage/LevelDbStorageService.cpp
  services/storage/LevelDbStorageService.h
  services/storage/StorageService.h
  workers/Worker.cpp
  workers/Worker.h
  workers/WorkerManager.cpp
  workers/WorkerManager.h
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
  ${CONAN_LIBS_boost}
  ${CONAN_LIBS_spdlog}
  ${CONAN_LIBS_fmt}
  ${CONAN_LIBS_leveldb}
  ${CONAN_LIBS}
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
