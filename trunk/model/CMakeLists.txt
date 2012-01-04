set(ItemName "model")

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
  target_link_libraries(${ItemName}
    render
    tinyxml2
    )
  set_target_properties(${ItemName} PROPERTIES 
  # DEFINE_SYMBOL "${ItemName}_EXPORTS"
  COMPILE_DEFINITIONS "ExportDynamicLibModel_"
  )
endif(StaticLibrary_)

# Add dependent external Libraries

include_directories(
  ${CMAKE_SOURCE_DIR}
)
