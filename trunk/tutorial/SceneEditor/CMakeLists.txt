set(ItemName "SceneEditor")

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
  ${CMAKE_SOURCE_DIR}/dependence/freetype-2.4.4/include
)
link_directories(
  ${dxPath}/lib/x86
)
add_executable(${ItemName} WIN32
  ${SRCS}
  ${HDRS}
  ${Res}
  LuaAPI.pkg
  toLua.bat
  ) 
if (StaticLibrary_)
  add_dependencies(${ItemName} bootstrapper misc render terrain scene tinyxml2 model)
  set_target_properties(${ItemName} PROPERTIES 
	COMPILE_DEFINITIONS "StaticLibrary_"
  )
else()
endif(StaticLibrary_)

# Add dependent external Libraries
target_link_libraries(${ItemName}
  winmm.lib
  d3d9.lib
  d3dx9.lib
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
  tinyxml2
  font
  ${CMAKE_SOURCE_DIR}/FreeImage/Dist/FreeImaged.lib
  )

add_custom_command(
  TARGET ${ItemName}
  PRE_BUILD
  COMMAND call toLua.bat
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )