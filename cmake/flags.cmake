##
## CMAKE file for neam/yaggler
##

set(YAGGLER_FLAGS "-fno-rtti -march=native -mtune=native")

# general flags
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(YAGGLER_FLAGS "${YAGGLER_FLAGS} -O0 -g3")
else(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(YAGGLER_FLAGS "${YAGGLER_FLAGS} -DNDEBUG -fmerge-all-constants")
else(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
  set(YAGGLER_FLAGS "${YAGGLER_FLAGS} -DNDEBUG -fmerge-all-constants")
endif()

# some gcc/clang flags
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(YAGGLER_FLAGS "${YAGGLER_FLAGS} -std=gnu++11 -Wall -Wextra -Wno-unused-function")

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(YAGGLER_FLAGS "${YAGGLER_FLAGS} -finline-limit=0")
  else(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(YAGGLER_FLAGS "${YAGGLER_FLAGS} -Ofast -fconserve-space -fno-enforce-eh-specs -finline-limit=5000 -fwhole-program -fmerge-constants -fmerge-all-constants -fmodulo-sched -fmodulo-sched-allow-regmoves -fgcse-sm -fgcse-las  -fgcse-after-reload")
  else(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(YAGGLER_FLAGS "${YAGGLER_FLAGS} -Os -fconserve-space -fno-enforce-eh-specs -finline-limit=5000 -fwhole-program -fmerge-constants -fmerge-all-constants -fmodulo-sched -fmodulo-sched-allow-regmoves -fgcse-sm -fgcse-las  -fgcse-after-reload")
  endif()

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(YAGGLER_FLAGS "${YAGGLER_FLAGS} -std=c++11 -Wall -Wextra -Wno-unused-function")

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(YAGGLER_FLAGS "${YAGGLER_FLAGS}")
  else(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(YAGGLER_FLAGS "${YAGGLER_FLAGS} -O3")
  else(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(YAGGLER_FLAGS "${YAGGLER_FLAGS} -Oz")
  endif()
endif()


