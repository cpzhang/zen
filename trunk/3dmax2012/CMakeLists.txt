SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../../bin)
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/external/freetype-2.4.4/Include
)
set(MaxSDK "e:/3dmax/3ds Max 2012 SDK/maxsdk")
SET(MaxPlugins  "e:/3dmax/3ds Max 2012/plugins/")
add_subdirectory(MaxExporter)
add_subdirectory(MaxExporterFB)
add_subdirectory(MaxGUP)
add_subdirectory(MaxMaterial)
add_subdirectory(MaxParticle)
#add_subdirectory(MaxPreview)