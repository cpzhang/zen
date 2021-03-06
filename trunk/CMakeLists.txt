# cmake_policy(VERSION major.minor[.patch])
cmake_minimum_required(VERSION 2.8)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

# Specifies the build type for make based generators
SET(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_FLAGS_RELEASE "/MD /Zi /Od")

set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /machine:x86" )

#add_definitions(-DUNICODE -D_UNICODE -D3D_DEBUG_INFO)

#静态库便于发布，运行速度较快；动态库编译方便，便于开发调试

set(StaticLibrary_ false)

if (StaticLibrary_)

  add_definitions(-StaticLibrary_)

else()

  # add_definitions(-ExportDynamicLib_)

endif(StaticLibrary_)
#
MESSAGE( STATUS "MSVC: " "${MSVC}" )
MESSAGE( STATUS "MSVC_IDE: " ${MSVC_IDE} )
MESSAGE( STATUS "MSVC60: " ${MSVC60} )
MESSAGE( STATUS "MSVC70: " ${MSVC70} )
MESSAGE( STATUS "MSVC71: " ${MSVC71} )
MESSAGE( STATUS "CMAKE_CXX_COMPILER_VERSION: " ${CMAKE_CXX_COMPILER_VERSION} )
MESSAGE( STATUS "CMAKE_COMPILER_2005: " ${CMAKE_COMPILER_2005} )
#IF(CMAKE_COMPILER_2005)
  set(dxPath "${CMAKE_SOURCE_DIR}/dependence/directx2005")
#else()
  #set(dxPath "${CMAKE_SOURCE_DIR}/dependence/directx2010")
#ENDIF(CMAKE_COMPILER_2005)

# project name
project(Zen)

set(vBinDir "d:/work/zen/bin")
SET(EXECUTABLE_OUTPUT_PATH ${vBinDir})
SET(LIBRARY_OUTPUT_PATH ${vBinDir})

# add_subdirectory(dependence)

add_subdirectory("misc")

set(RenderName "render")
add_subdirectory(render)

add_subdirectory(lua)
add_subdirectory(tolua-5.2.0)
add_subdirectory(luaScript)
# add_subdirectory("terrain")
add_subdirectory("tinyxml2")
add_subdirectory("scene")
add_subdirectory("model")
add_subdirectory("font")
#add_subdirectory("client")

# add_subdirectory(tool)

#add_subdirectory(3dmax2012)

add_subdirectory(tutorial)

ADD_SUBDIRECTORY(recast/Detour)
#ADD_SUBDIRECTORY(DetourTileCache)
ADD_SUBDIRECTORY(recast/Recast)