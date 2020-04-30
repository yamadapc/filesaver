find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)

set (
  SOURCE_FILES
  main.cpp
  widgets/MainWindow.h
  widgets/MainWindow.cpp
)
list(TRANSFORM SOURCE_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

add_executable (
  filesaver_gui
  ${SOURCE_FILES}
)
target_include_directories(
  filesaver_gui
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(
  filesaver_gui
  lfilesaver
  Qt5::Widgets
  ${CONAN_LIBS}
  pthread
  dl
  leveldb
)
