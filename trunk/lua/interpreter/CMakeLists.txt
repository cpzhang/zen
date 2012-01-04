set(ItemName "lua_interpreter")

link_directories(
  ${LIBRARY_OUTPUT_PATH}
  )
add_executable(${ItemName}
 ../src/lua.c
  )

target_link_libraries(${ItemName}
    lua_library.lib
  )