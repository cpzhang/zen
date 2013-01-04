set(ItemName "toLuaExecute")

link_directories(
  ${LIBRARY_OUTPUT_PATH}
  )
add_executable(${ItemName}
../src/bin/tolua.c
../src/bin/toluabind.c
  )

link_directories(
  ${EXECUTABLE_OUTPUT_PATH}
  )

target_link_libraries(${ItemName}
    lua_library.lib
    toLuaLibrary.lib	
  )

include_directories(
  ${CMAKE_SOURCE_DIR}/lua/src
  ../include
)
