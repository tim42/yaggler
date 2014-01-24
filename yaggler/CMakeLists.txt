##
## CMAKE file for neam/yaggler
##


cmake_minimum_required(VERSION 2.8)

include_directories(./)

# version changing
add_custom_target(invalidate_version ALL COMMAND ${CMAKE_COMMAND} -E touch version.hpp WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# I build an ~empty lib, just to allow kdevelop to parse correctly these files
#
# [here I could not use 'set(...)', and I had to use file(GLOB_RECURSE ...) to
#  have kdevelop parsing and pushing include directories]
file(GLOB_RECURSE srcs ./*.cpp)


# set(YAGGLER_SRCS ${srcs} PARENT_SCOPE)

add_definitions(${YAGGLER_FLAGS})

# and "build" yaggler
add_library(${PROJ_APP} STATIC ${srcs})
add_dependencies(${PROJ_APP} invalidate_version)

