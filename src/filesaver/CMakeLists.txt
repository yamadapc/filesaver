add_executable (
  filesaver
  main.cpp
  command_line/CommandLineApp.cpp
  command_line/CommandLineApp.h
  command_line/logger/StatusDescr.cpp
  command_line/logger/StatusDescr.h
  command_line/logger/StatusPrinter.cpp
  command_line/logger/StatusPrinter.h
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
