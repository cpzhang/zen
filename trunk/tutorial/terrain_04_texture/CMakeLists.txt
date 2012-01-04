set(ItemName "terrain_04_texture")

FILE(GLOB SRCS 
  ${CMAKE_SOURCE_DIR}/dependence/FileTreeCtrl/*.*
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
  ${CMAKE_SOURCE_DIR}/tolua-5.2.0/include
  ${CMAKE_SOURCE_DIR}/lua/src
)

add_executable(${ItemName} WIN32
  ${SRCS}
  ${HDRS}
  ${Res}
  LuaAPI.pkg
  toLua.bat
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
  luaScript.lib
  lua_library.lib
  toLuaLibrary.lib
  # luaAPI.lib
  scene.lib
  # terrain.lib
  model
  )

add_custom_command(
  TARGET ${ItemName}
  PRE_BUILD
  COMMAND call toLua.bat
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )