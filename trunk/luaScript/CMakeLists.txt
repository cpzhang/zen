set(ItemName "luaScript")

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
  add_dependencies(${ItemName} misc lua_library)
  set_target_properties(${ItemName} PROPERTIES 
  	COMPILE_DEFINITIONS "StaticLibrary_"
    )
else()
	link_directories(
		${EXECUTABLE_OUTPUT_PATH}
	)

  add_library(${ItemName} SHARED
    ${SRCS} 
    ${HDRS}
    )
  set_target_properties(${ItemName} PROPERTIES 
    # DEFINE_SYMBOL "${ItemName}_EXPORTS"
    COMPILE_DEFINITIONS "ExportDynamicLibLuaScript_"
    )
    target_link_libraries(${ItemName}
		lua_library
	)
endif(StaticLibrary_)


include_directories(
  ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/tolua-5.2.0/include
    ${CMAKE_SOURCE_DIR}/lua/src
)
