##
## CMAKE file for neam/yaggler
##

cmake_minimum_required(VERSION 2.8)


set(PROJ_VERSION_MAJOR 0)
set(PROJ_VERSION_MINOR 0)
set(PROJ_VERSION_SUPERMINOR 1)

add_definitions(-D_PROJ_VERSION_MAJOR=${PROJ_VERSION_MAJOR})
add_definitions(-D_PROJ_VERSION_MINOR=${PROJ_VERSION_MINOR})
add_definitions(-D_PROJ_VERSION_SUPERMINOR=${PROJ_VERSION_SUPERMINOR})

set(PROJ_VERSION_NAME duck)

set(PROJ_NAME yaggler)

set(PROJ_APP ${PROJ_NAME}-${PROJ_VERSION_NAME}-${PROJ_VERSION_MAJOR}.${PROJ_VERSION_MINOR}.${PROJ_VERSION_SUPERMINOR})
project(${PROJ_APP})

message(STATUS "-----------------------------------------------")
message(STATUS "Building neam/${PROJ_NAME} :: ${PROJ_APP}")
message(STATUS "-----------------------------------------------")
message(STATUS "")
message(STATUS "")


if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release MinSizeRel." FORCE)
endif ()

if (${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
  # And this screw up users.
  # YAY :D
  message(STATUS "when checking: '${CMAKE_CURRENT_BINARY_DIR}' is the same as '${PROJECT_SOURCE_DIR}'")
  message(FATAL_ERROR "you must create a build folder in order to build ${PROJ_APP} !!!")
endif()

# deps check
include(cmake/deps.cmake)

# output
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/")
set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/libs")

set(YAGGLER_SRCS "")

# build some third party libs
add_subdirectory("./third_party/glew")
add_subdirectory("./third_party/glfw")

# include those third party libs
include_directories(SYSTEM "./third_party/glfw/include/")
include_directories(SYSTEM "./third_party/glew/include/")
include_directories(SYSTEM "./third_party/png++/include/")
include_directories(SYSTEM "./third_party/glm/")

# create the YAGGLER_FLAGS variable
include(cmake/flags.cmake)

include_directories("./yaggler/")
include_directories("./")

# build yaggler dummy "lib"
add_subdirectory(yaggler)

# build the samples
add_subdirectory(samples/test)
add_subdirectory(samples/dev)
add_subdirectory(samples/dev-klmb)



# copy
file(COPY ./data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
