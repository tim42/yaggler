
YägGLer ship some libraries, that, in some parts, are modified to fulfill some
purposes (or to simply work).

Modified libs are:
  - GLFW: The only modification at this date (23/01/2014) is: modification of
    the default values of options in the CMakeLists.txt at the root of the
    projet.

  - GLEW: Added a CMakeLists.txt to be built with YägGler, removed unused
    directories/build system, added an include directory.
    There's is also a fix in glew.c

