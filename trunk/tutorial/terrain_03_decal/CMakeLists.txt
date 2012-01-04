set(ItemName "terrain_03_decal")

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
  ${CMAKE_SOURCE_DIR}/wtl81/Include
)

add_executable(${ItemName} WIN32
  ${SRCS}
  ${HDRS}
  ${Res}
  ) 
if (StaticLibrary_)
  add_dependencies(${ItemName} bootstrapper misc render terrain scene)
else()
endif(StaticLibrary_)
# Add dependent external Libraries
target_link_libraries(${ItemName}
  winmm.lib
  # d3d9.lib
  # d3dx9.lib
  # client.lib
  render.lib
  misc.lib
  # luaScript.lib
  # lua_library.lib
  # toLuaLibrary.lib
  # luaAPI.lib
  scene.lib
  # terrain.lib
  )