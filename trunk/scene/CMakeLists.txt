set(ItemName "scene")

FILE(GLOB SRCS
  *.cpp 
) 

FILE(GLOB HDRS
  *.h
)
include_directories(
  ${CMAKE_SOURCE_DIR}
)
link_directories(
  ${CMAKE_SOURCE_DIR}/dependence/directx/lib/x86
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
  target_link_libraries(${ItemName}
    render
    tinyxml2
    model
    )
  set_target_properties(${ItemName} PROPERTIES 
  # DEFINE_SYMBOL "${ItemName}_EXPORTS"
  COMPILE_DEFINITIONS "ExportDynamicLibScene_"
  )
endif(StaticLibrary_)

# Add dependent external Libraries

