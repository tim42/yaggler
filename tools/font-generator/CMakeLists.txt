##
## CMAKE file for neam/yaggler dev sample
##

cmake_minimum_required(VERSION 2.8)

include(FindFreetype)

if(FREETYPE_FOUND)
  # set the name of the sample
  set(SAMPLE_NAME "font-generator_${PROJ_APP}")

  # avoid listing all the files
  file(GLOB_RECURSE srcs ./*.cpp)

  add_definitions(${YAGGLER_FLAGS})

  include_directories(${FREETYPE_INCLUDE_DIRS})

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${YAGGLER_FLAGS}")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${YAGGLER_FLAGS}")

  add_executable(${SAMPLE_NAME} ${srcs} ${YAGGLER_SRCS})

  target_link_libraries(${SAMPLE_NAME} ${YAGGLER_DEPS_LIBS} ${FREETYPE_LIBRARIES})

  add_dependencies(${SAMPLE_NAME} ${PROJ_APP})
else ()
  message(WARNING "Disabling the font-generator: missing FreeType library.")
endif ()
