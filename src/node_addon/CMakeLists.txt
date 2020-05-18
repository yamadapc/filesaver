if (CMAKE_JS_VERSION)
else()
  set(
    CMAKE_JS_INC
    ~/.nvm/versions/node/v14.0.0/include/node/
  )
  message("-- Fallback include to ${CMAKE_JS_INC}")
endif()

set(
  NODE_SOURCE_FILES
  main.cpp
)
add_library(node_filesaver SHARED ${NODE_SOURCE_FILES} ${CMAKE_JS_SRC})
target_include_directories(
  node_filesaver
  PRIVATE
  SYSTEM
  ${CMAKE_JS_INC}
  ${CMAKE_CURRENT_LIST_DIR}/../
)
set_target_properties(node_filesaver PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(
  node_filesaver
  lfilesaver
  ${CMAKE_JS_LIB}
)
