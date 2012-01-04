set(ItemName "toLuaLibrary")

link_directories(
  ${LIBRARY_OUTPUT_PATH}
  )
add_library(${ItemName}
../src/lib/tolua_event.h
../src/lib/tolua_event.c
../src/lib/tolua_is.c
../src/lib/tolua_map.c
../src/lib/tolua_push.c
../src/lib/tolua_to.c
  )

#target_link_libraries(${ItemName}
 #   lua_library.lib
  #)

include_directories(
  ${CMAKE_SOURCE_DIR}/lua/src
  ../include
)
