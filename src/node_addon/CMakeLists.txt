include_directories(${CMAKE_JS_INC})

file(GLOB SOURCE_FILES "./")

add_library(
  node_filesaver
  SHARED
  ${SOURCE_FILES}
  ${CMAKE_JS_SRC}
)
set_target_properties(node_filesaver PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(node_filesaver ${CMAKE_JS_LIB})
