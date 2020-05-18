add_executable (
  filesaver
  main.cpp
  command-line/CommandLineApp.cpp
  command-line/CommandLineApp.h
  command-line/logger/StatusDescr.cpp
  command-line/logger/StatusDescr.h
  command-line/logger/StatusPrinter.cpp
  command-line/logger/StatusPrinter.h
)
target_include_directories(
  filesaver
  PRIVATE
  SYSTEM
  ${CMAKE_CURRENT_LIST_DIR}/../
)
target_compile_options (filesaver PRIVATE ${PROJECT_WARNINGS})
target_link_libraries (
  filesaver
  project_warnings
  project_options
  lfilesaver
)
