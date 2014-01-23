##
## CMAKE file for neam/yaggler
##


cmake_minimum_required(VERSION 2.8)

include_directories(./include/)

# avoid listing all the files
file(GLOB_RECURSE srcs
  ./glew.c
)

# and build yaggler
add_library(glew SHARED ${srcs})

