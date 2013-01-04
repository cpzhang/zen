set(ItemName ${RenderName})

FILE(GLOB SRCS
  *.cpp 
) 

FILE(GLOB HDRS
  *.h
)

FILE(GLOB FX
  ${vDataDir}/shader/*.fx*
) 

SOURCE_GROUP(FX FILES ${FX})

include_directories(
  ${CMAKE_SOURCE_DIR}
  # ${CMAKE_SOURCE_DIR}/external/freetype-2.4.4/Include
  ${CMAKE_SOURCE_DIR}/dependence/directx/Include
)

link_directories(
  # ${LIBRARY_OUTPUT_PATH}
  ${CMAKE_SOURCE_DIR}/dependence/directx/lib/x86
  # ${CMAKE_SOURCE_DIR}/external/freetype-2.4.4/lib
  # ${CMAKE_SOURCE_DIR}/external/zlib/lib
  )

if (StaticLibrary_)
  add_library(${ItemName} STATIC
    ${SRCS} 
    ${HDRS}
    ${FX}
    ) 
  add_dependencies(${ItemName} misc)
  set_target_properties(${ItemName} PROPERTIES 
	COMPILE_DEFINITIONS "StaticLibrary_"
  )
else()
  add_library(${ItemName} SHARED
    ${SRCS} 
    ${HDRS}
    ${FX}
    )
  target_link_libraries(${ItemName}
    # "dxerr.lib"
    "d3d9.lib"
    "d3dx9.lib"
    "misc"
    # "winmm"
    # "freetype.lib"
    # "Math.lib"
    # "Base.lib"
    # "zlib.lib"
    # tinyxml2.lib
 )
set_target_properties(${ItemName} PROPERTIES 
  # DEFINE_SYMBOL "${ItemName}_EXPORTS"
  COMPILE_DEFINITIONS "ExportDynamicLibRender_"
  )
endif(StaticLibrary_)

# add_dependencies(${ItemName} misc)
# Add dependent external Libraries
#target_link_libraries(${ItemName}
#  "dxerr.lib"
#  "d3d9.lib"
#  "d3dx9.lib"
  # "winmm"
  # "freetype.lib"
  # "Math.lib"
  # "Base.lib"
  # "zlib.lib"
  # tinyxml2.lib
 # )

# set_target_properties(${ItemName} PROPERTIES 
# DEFINE_SYMBOL "${ItemName}_EXPORTS"
	# COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE;D3D_DEBUG_INFO"
# COMPILE_DEFINITIONS "NV_PERF_HUD"
#COMPILE_FLAGS "/Zp1"
# )
