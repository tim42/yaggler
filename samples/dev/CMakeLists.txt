##
## CMAKE file for neam/yaggler dev sample
##

cmake_minimum_required(VERSION 2.8)

# avoid listing all the files
file(GLOB_RECURSE srcs ./*.cpp)

# general flags
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(FLAGS "-O0 -g3 -fno-rtti  -DNDEBUG -finline-limit=5000 -march=native -mtune=native")
else(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(FLAGS "-fno-rtti -DNDEBUG -fmerge-all-constants -march=native -mtune=native")
else(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
  set(FLAGS "-fno-rtti -DNDEBUG -flto -fmerge-all-constants -march=native -mtune=native")
endif()

# some gcc/clang flags
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  add_definitions(-std=gnu++11 -Wall -Wextra)

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(FLAGS "${FLAGS} -O0 -g3 -DNDEBUG -finline-limit=0")
  else(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(FLAGS "${FLAGS} -Ofast -fconserve-space -fno-enforce-eh-specs -finline-limit=5000 -fwhole-program -fmerge-constants -fmerge-all-constants -fmodulo-sched -fmodulo-sched-allow-regmoves -fgcse-sm -fgcse-las  -fgcse-after-reload")
  else(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(FLAGS "${FLAGS} -Os -fconserve-space -fno-enforce-eh-specs -finline-limit=5000 -fwhole-program -fmerge-constants -fmerge-all-constants -fmodulo-sched -fmodulo-sched-allow-regmoves -fgcse-sm -fgcse-las  -fgcse-after-reload")
  endif()

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  add_definitions(-std=c++11 -Wall -Wextra)

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(FLAGS "${FLAGS}")
  else(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(FLAGS "${FLAGS} -O3")
  else(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(FLAGS "${FLAGS} -Oz")
  endif()
endif()

add_definitions(${FLAGS})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${FLAGS}")

add_executable(dev-${PROJ_APP} ${srcs} ${YAGGLER_SRCS})

target_link_libraries(dev-${PROJ_APP} ${YAGGLER_DEPS_LIBS} glew)

# add_dependencies(dev-${PROJ_APP} ${PROJ_APP})

