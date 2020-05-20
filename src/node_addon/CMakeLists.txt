function (add_node_addon)
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
endfunction()

if (CMAKE_JS_VERSION)
  message("-- CMAKE_JS_VERSION: ${CMAKE_JS_VERSION}")
  message("-- CMAKE_JS_INC: ${CMAKE_JS_INC}")
  message("-- CMAKE_JS_LIB: ${CMAKE_JS_LIB}")
  message("-- CMAKE_JS_SRC: ${CMAKE_JS_SRC}")
  add_node_addon()
elseif(MOCK_CMAKE_JS)
  set(
    CMAKE_JS_INC
    ~/.cmake-js/node-x64/v14.0.0/include/node/
    ${CMAKE_CURRENT_LIST_DIR}/../../node_modules/nan
  )
  message("-- Fallback include to ${CMAKE_JS_INC}")
  set(NODE_RUNTIME "node")
  set(NODE_RUNTIMEVERSION "14.0.0")
  set(NODE_ARCH "x64")
  set(WARNINGS_AS_ERRORS "OFF")
  set(CMAKE_CXX_FLAGS "-D_DARWIN_USE_64_BIT_INODE=1 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -DBUILDING_NODE_EXTENSION")
  set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
  add_node_addon()
else()
  message("-- Skipping node addon build")
endif()
