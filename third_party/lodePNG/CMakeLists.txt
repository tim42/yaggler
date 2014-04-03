
# simple cmake file to build a static lodePNG library.
project(lodePNG)

include_directories("include/lodePNG/")

# and the library
add_library(lodePNG STATIC src/lodepng.cpp)

