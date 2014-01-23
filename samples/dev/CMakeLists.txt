##
## CMAKE file for neam/yaggler dev sample
##

cmake_minimum_required(VERSION 2.8)

# avoid listing all the files
file(GLOB_RECURSE srcs ./*.cpp)

add_definitions(${YAGGLER_FLAGS})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${YAGGLER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${YAGGLER_FLAGS}")

add_executable(dev-${PROJ_APP} ${srcs} ${YAGGLER_SRCS})

target_link_libraries(dev-${PROJ_APP} ${YAGGLER_DEPS_LIBS} glew)

add_dependencies(dev-${PROJ_APP} ${PROJ_APP})

