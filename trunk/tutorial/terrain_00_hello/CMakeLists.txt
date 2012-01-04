set(ItemName "terrain_00_hello")

FILE(GLOB SRCS 
  *.cpp 
) 

FILE(GLOB HDRS 
  *.h
) 
FILE(GLOB Res
  *.rc
) 
link_directories(
  ${EXECUTABLE_OUTPUT_PATH}
  )

include_directories(
  ${CMAKE_SOURCE_DIR}
  # ${CMAKE_SOURCE_DIR}/external/freetype-2.4.4/Include
)

add_executable(${ItemName} WIN32
  ${SRCS}
  ${HDRS}
  ${Res}
  ) 
add_dependencies(${ItemName} bootstrapper misc render)
# Add dependent external Libraries
target_link_libraries(${ItemName}
  winmm.lib
  d3d9.lib
  d3dx9.lib
  client.lib
  render.lib
  misc.lib
  luaScript.lib
  lua_library.lib
  toLuaLibrary.lib
  luaAPI.lib
  )