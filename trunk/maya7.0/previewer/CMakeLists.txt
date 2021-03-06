set(ItemName "MayaPreviewer")

FILE(GLOB SRCS
  *.cpp 
) 

FILE(GLOB HDRS
  *.h
)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${MayaSDKInclude}
)

link_directories(
  ${LIBRARY_OUTPUT_PATH}
  ${MayaSDKLib}
  )
# If we build for windows systems, we also include the resource file containing the manifest, icon and other resources
if(WIN32)
  set(SRCS ${SRCS} *.rc)
endif(WIN32)
 
# Here we define the executable minimal.exe or minimal on other systems
# the above paths and defines will be used in this build
add_executable(${ItemName} WIN32
  ${SRCS} 
  ${HDRS}
  ${SRCS}
  ) 

# Add dependent external Libraries
target_link_libraries(${ItemName}
  "Math.lib"
  "Base.lib"
  Foundation.lib 
  OpenMaya.lib 
  OpenMayaAnim.lib 
  OpenMayaRender.lib 
  Version.lib
  )

set_target_properties(${ItemName} PROPERTIES 
# SUFFIX ".dle"
COMPILE_DEFINITIONS "_CRT_SECURE_NO_DEPRECATE;"
)

add_custom_command(
  TARGET ${ItemName}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "${LIBRARY_OUTPUT_PATH}/debug/${ItemName}.pdb" "../${MaxPlugins}/previewer."
  COMMAND ${CMAKE_COMMAND} -E copy "${LIBRARY_OUTPUT_PATH}/debug/${ItemName}.exe" "../${MaxPlugins}/previewer."
  )