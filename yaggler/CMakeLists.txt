##
## CMAKE file for neam/yaggler
##


cmake_minimum_required(VERSION 2.8)

include_directories(./)

# version changing
add_custom_target(invalidate_version ALL COMMAND ${CMAKE_COMMAND} -E touch version.hpp WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(srcs
)

set(YAGGLER_SRCS ${srcs} PARENT_SCOPE)

# and build yaggler
# add_library(${PROJ_APP} ${srcs})
# add_dependencies(${PROJ_APP} invalidate_version)

