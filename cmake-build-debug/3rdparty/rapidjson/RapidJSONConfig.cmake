

include ("${CMAKE_CURRENT_LIST_DIR}/RapidJSON-targets.cmake")

################################################################################
# RapidJSON source dir
set( RapidJSON_SOURCE_DIR "/home/czq/Project/3D_Modeling/RenderEngine/3rdparty/rapidjson")

################################################################################
# RapidJSON build dir
set( RapidJSON_DIR "/home/czq/Project/3D_Modeling/RenderEngine/cmake-build-debug/3rdparty/rapidjson")

################################################################################
# Compute paths
get_filename_component(RapidJSON_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

get_target_property(RapidJSON_INCLUDE_DIR RapidJSON INTERFACE_INCLUDE_DIRECTORIES)

set( RapidJSON_INCLUDE_DIRS ${RapidJSON_INCLUDE_DIR} )
