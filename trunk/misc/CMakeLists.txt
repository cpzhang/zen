set(ItemName "misc")

FILE(GLOB SRCS
  *.cpp 
) 

FILE(GLOB HDRS
  *.h
)

if (StaticLibrary_)
  add_library(${ItemName} STATIC
  ${SRCS} 
  ${HDRS}
  ) 
  set_target_properties(${ItemName} PROPERTIES 
	COMPILE_DEFINITIONS "StaticLibrary_"
  )
else()
  add_library(${ItemName} SHARED
  ${SRCS} 
  ${HDRS}
  ) 
set_target_properties(${ItemName} PROPERTIES 
  # DEFINE_SYMBOL "${ItemName}_EXPORTS"
  COMPILE_DEFINITIONS "ExportDynamicLibMisc_"
  )
endif(StaticLibrary_)

# Add dependent external Libraries

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${dxPath}/Include
  )
