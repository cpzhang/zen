set(ItemName "font")

FILE(GLOB SRCS
  *.cpp 
) 

FILE(GLOB HDRS
  *.h
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
    ${CMAKE_SOURCE_DIR}/dependence/freetype-2.4.4/lib/freetype.lib
    )
  set_target_properties(${ItemName} PROPERTIES 
  # DEFINE_SYMBOL "${ItemName}_EXPORTS"
  COMPILE_DEFINITIONS "ExportDynamicLibFont_"
  )
endif(StaticLibrary_)

# Add dependent external Libraries

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/dependence/freetype-2.4.4/include
)
