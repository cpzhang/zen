set(ItemName "lua_compiler")

link_directories(
  ${LIBRARY_OUTPUT_PATH}
  )
add_executable(${ItemName}
 ../src/luac.c
  )

target_link_libraries(${ItemName}
    lua_library.lib
  )