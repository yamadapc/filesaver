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
  models/Settings.cpp
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
target_link_libraries (
  lfilesaver
  project_warnings
  project_options
  CONAN_PKG::boost
  CONAN_PKG::spdlog
  CONAN_PKG::fmt
  CONAN_PKG::leveldb
  pthread
  dl
)
