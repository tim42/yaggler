
include(FindOpenGL)
# include(FindGLEW)

# check OenGLEU
if (OPENGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIR})
else (OPENGL_FOUND)
  message(FATAL_ERROR "could not find OPENGL headers")
endif(OPENGL_FOUND)

# check GLEW
# if (GLEW_FOUND)
#   include_directories(${GLEW_INCLUDE_DIR})
# else (GLEW_FOUND)
#   message(FATAL_ERROR "could not find GLEW")
# endif(GLEW_FOUND)

# yaggler deps libs (for exec)
set(YAGGLER_DEPS_LIBS ${OPENGL_gl_LIBRARY} glfw png)
