
YägGLer ship some libraries, that, in some parts, are modified to fulfill some
purposes (described below).

Modified libs are:
  - GLFW: The only modification at this date (23/01/2014) is: modification of
    the default values of options in the CMakeLists.txt at the root of the
    projet.
    Added another modification to src/CMakeLists.txt to be compliant with the CMake's CMP022 policy. (03/02/2015)

  - GLEW: Added a CMakeLists.txt to be built with YägGler, removed unused
    directories/build system, added an include directory.
    There's is also a fix in glew.c

  - GLM: removed unused directories/CMakeFiles.

  - lodePNG: created a simple CMakeLists.txt to create a static library.
    Added a LICENSE file.

I do not own those libs, see the copying/license file in their folders.
You can download the original version on their respective author/official
website.
