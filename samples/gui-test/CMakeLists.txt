##
## CMAKE file for neam/yaggler dev sample
##

cmake_minimum_required(VERSION 2.8)

# set the name of the sample
set(SAMPLE_NAME "gui-test-${PROJ_APP}")

# avoid listing all the files
file(GLOB_RECURSE srcs ./*.cpp)

add_definitions(${YAGGLER_FLAGS})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${YAGGLER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${YAGGLER_FLAGS}")

add_executable(${SAMPLE_NAME} ${srcs} ${YAGGLER_SRCS})

target_link_libraries(${SAMPLE_NAME} ${YAGGLER_DEPS_LIBS})

add_dependencies(${SAMPLE_NAME} ${PROJ_APP})

