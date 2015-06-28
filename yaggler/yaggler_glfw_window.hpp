//
// file : yaggler_init_glut_window.hpp
// in : file:///home/tim/projects/yaggler/yaggler/yaggler_init_glut_window.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 08/10/2013 18:25:42
//
//
// Copyright (C) 2013-2014 Timothée Feuillet
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//


#ifndef __N_15663693851872442693_772963530__YAGGLER_GLFW_WINDOW_HPP__
# define __N_15663693851872442693_772963530__YAGGLER_GLFW_WINDOW_HPP__

#include <string>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "ct_point.hpp"
#include "yaggler_except.hpp"
#include "setup.hpp"
#include "tools/ownership.hpp"
#include "tools/backtrace.hpp"

namespace neam
{
  namespace yaggler
  {
    /// \brief list the window creation modes
    namespace window_mode
    {
      /// \brief a fullscreen window
      class fullscreen_t {} fullscreen __attribute__((unused));

      /// \brief a not-fullscreen window
      class windowed_t {} windowed __attribute__((unused));
    } // namespace window_mode

    /// \brief an helper class for creating windows with GLFW
    class glfw_window
    {
      private:
        void init_glfw_hints()
        {
          ::opengl_version::_log();

          glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ::opengl_version::gl_major);
          glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ::opengl_version::gl_minor);

          // switch over a constexpr value: no overhead/no generaed branchement code.
          if (::opengl_version::gl_major > 3 || (::opengl_version::gl_major == 3 && ::opengl_version::gl_minor >= 2))
          {
            switch (::opengl_version::profile)
            {
              case setup::opengl_profile::any:
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
                break;
              case setup::opengl_profile::core:
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                break;
              case setup::opengl_profile::compat:
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
                break;
            }
            // some additional hints
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, ::opengl_version::forward_compat);
          }

          glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, ::opengl_version::debug);
        }

        void init_debug()
        {
          if (::opengl_version::debug)
          {
            if (GLEW_ARB_debug_output)
            {
              glDebugMessageCallbackARB([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const char * message, void *) -> void
              {
                auto &stream = neam::cr::out.debug() << LOGGER_INFO;
                bool do_print_callstack = false;
                stream << "OPENGL DEBUG OUTPUT: [";
                switch (type)
                {
                  case GL_DEBUG_TYPE_ERROR_ARB:
                    stream << "ERROR";
                    do_print_callstack = true;
                    break;
                  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
                    stream << "DEPRECATED BEHAVIOUR";
                    break;
                  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
                    stream << "UNDEFINED BEHAVIOUR";
                    break;
                  case GL_DEBUG_TYPE_PORTABILITY_ARB:
                    stream << "PORTABILITY ISSUE";
                    break;
                  case GL_DEBUG_TYPE_PERFORMANCE_ARB:
                    stream << "PERFORMANCE ISSUE";
                    break;
                  case GL_DEBUG_TYPE_MARKER:
                    stream << "ANNOTATION";
                    break;
                  case GL_DEBUG_TYPE_PUSH_GROUP:
                    stream << "PUSH";
                    break;
                  case GL_DEBUG_TYPE_POP_GROUP:
                    stream << "POP";
                    break;
                  case GL_DEBUG_TYPE_OTHER_ARB:
                    stream << "OTHER(" << type << ")";
                }
                stream << "/";
                switch (severity)
                {
                  case GL_DEBUG_SEVERITY_HIGH_ARB:
                    stream << "HIGH";
                    do_print_callstack = true;
                    break;
                  case GL_DEBUG_SEVERITY_MEDIUM_ARB:
                    stream << "MEDIUM";
                    break;
                  case GL_DEBUG_SEVERITY_LOW_ARB:
                    stream << "LOW";
                    break;
                  case GL_DEBUG_SEVERITY_NOTIFICATION:
                    stream << "NOTIF";
                    break;
                  default:
                    stream << "[" << severity << "]";
                }
                stream << "] from [";
                switch (source)
                {
                  case GL_DEBUG_SOURCE_API_ARB:
                    stream << "API";
                    break;
                  case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
                    stream << "WINDOW SYSTEM";
                    break;
                  case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
                    stream << "SHADER COMPILER";
                    break;
                  case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
                    stream << "THIRD PARTY";
                    break;
                  case GL_DEBUG_SOURCE_APPLICATION_ARB:
                    stream << "APPLICATION";
                    break;
                  case GL_DEBUG_SOURCE_OTHER_ARB:
                    stream << "OTHER (" << source << ")";
                }
                stream << "] (ID: " << id << "): '" << message << "'" << std::endl;

                if (do_print_callstack)
                  neam::cr::print_callstack(25, 2);
              }, nullptr);

              glEnable(GL_DEBUG_OUTPUT);
              glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
              glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

              glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW_ARB, 0, NULL, true);
              glDebugMessageControlARB(GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
              glDebugMessageControlARB(GL_DEBUG_SOURCE_THIRD_PARTY_ARB, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
            }
          }
        }

      public:
        /// \brief create a non-fullscreen window
        /// \param window_size the size of the window in pixel. It MUST be an integer, NOT a fixed point size.
        /// \param w_hints is a list of additional window creation hints as handled by GLFW
        ///       \code {{HINT_NAME, HINT_VALUE}, {HINT_NAME2, HINT_VALUE2}, ...} \endcode
        glfw_window(window_mode::windowed_t, const neam::ct::vector2 &window_size, const std::string &title = "[ neam/yaggler",
                    std::initializer_list<std::pair<int, int>> w_hints = std::initializer_list<std::pair<int, int>>())
          : win(nullptr), link(false)
        {
          init_glfw_hints();
          // init additional hints
          for (auto &it : w_hints)
            glfwWindowHint(it.first, it.second);

          if (!(win = glfwCreateWindow(window_size.x, window_size.y, title.data(), 0, 0)))
            throw glfw_exception("GLFW: glfwCreateWindow call failed");

          select();

          // init glew
          glewExperimental = GL_TRUE;
          GLenum err = glewInit();

          throw_on_glerror<glew_exception>("GLEW INTERNAL ERROR (glewInit): ");
          if (err != GLEW_OK)
          {
            throw glew_exception(reinterpret_cast<const char *>(glewGetErrorString(err)));
          }

          init_debug();
        }

        /// \brief create a fullscreen window
        /// \note the window size is deduced from the resolution of the primary monitor
        /// \param w_hints is a list of additional window creation hints as handled by GLFW
        ///       \code {{HINT_NAME, HINT_VALUE}, {HINT_NAME2, HINT_VALUE2}, ...} \endcode
        glfw_window(window_mode::fullscreen_t, const std::string &title = "[ neam/yaggler",
                    std::initializer_list<std::pair<int, int>> w_hints = std::initializer_list<std::pair<int, int>>())
        : win(nullptr), link(false)
        {
          init_glfw_hints();
          // init additional hints
          for (auto & it : w_hints)
            glfwWindowHint(it.first, it.second);

          const GLFWvidmode *vmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

          if (!(win = glfwCreateWindow(vmode->width, vmode->height, title.data(), glfwGetPrimaryMonitor(), 0)))
            throw glfw_exception("GLFW: glfwCreateWindow call failed");

          select();

          // init glew
          glewExperimental = GL_TRUE;
          GLenum err = glewInit();

          throw_on_glerror<glew_exception>("GLEW INTERNAL ERROR (glewInit): ");
          if (err != GLEW_OK)
          {
            throw glew_exception(reinterpret_cast<const char *>(glewGetErrorString(err)));
          }

          init_debug();
        }

        /// \brief initialize a new window instance that will be related to an existing GLFW window
        /// \note the new window instance will not have ownership on the window: destructing the instance won't destruct the window
        /// \note this will not "copy" the window (it wouldn't mean anything)
        glfw_window(const glfw_window &w)
          : win(w.win), link(true)
        {
        }

        /// \brief setup an existing instance to be related to an existing GLFW window
        /// \note the instance will not have ownership on the window: destructing the instance won't destruct the window
        /// \note this will not "copy" the window (it wouldn't mean anything)
        glfw_window &operator = (const glfw_window &w)
        {
          if (this != &w && win && !link)
            glfwDestroyWindow(win);
          win = w.win;
          link = true;
          return *this;
        }

        /// \brief setup an existing instance to be related to an existing GLFW window,
        ///        but if the other windows instance has the ownership over the GLFW window,
        ///        the new instance will have ownership of the GLFW window
        ///        (destructing the instance will destroy the GLFW window)
        glfw_window(glfw_window &w, stole_ownership_t) : win(w.win), link(w.link)
        {
          w.link = true;
        }

        /// \brief construct from a temporary window
        /// \note \p w will still be usable after the operation
        glfw_window(glfw_window &&w) : win(w.win), link(w.link)
        {
          w.link = true;
        }

        /// \brief transfer the ownership of a GLFW window instance
        /// \note \p w will still be usable after the operation
        glfw_window &operator = (glfw_window &&w)
        {
          if (this != &w)
          {
            if (win && !link)
              glfwDestroyWindow(win);
            win = w.win;
            link = w.link;
            w.link = true;
          }
          return *this;
        }

        /// \brief destroy the window
        ~glfw_window()
        {
          if (win && !link)
            glfwDestroyWindow(win);
        }

        /// \brief abandon the overniship of a GLFW window.
        /// This mostly mean that destructing the instance won't close the window
        glfw_window &give_up_ownership()
        {
          link = true;
          return *this;
        }

        /// \brief assume the overniship of a GLFW window.
        /// This mostly mean that destructing the instance WILL close the window
        glfw_window &assume_ownership()
        {
          link = false;
          return *this;
        }

        /// \brief return the ownership status of the current instance over the underlying GLFW window
        bool has_ownership() const
        {
          return !link;
        }

        /// \brief Makes the context of the specified window current for the calling thread.
        /// This function makes the context of the specified window current on the
        /// calling thread.  A context can only be made current on a single thread at
        /// a time and each thread can have only a single current context at a time.
        /// (from the GLFW documentation for glfwMakeContextCurrent())
        /// \see glfwMakeContextCurrent()
        void select() const
        {
          glfwMakeContextCurrent(win);
        }

        /// \brief resize the window
        /// \param window_size the new window size. MUST be an integer, NOT a fixed point size.
        void set_size(const neam::ct::vector2 &window_size)
        {
          glfwSetWindowSize(win, window_size.x, window_size.y);
        }

        /// \brief Return the current size of the window
        /// \note the returned vector2 is NOT in fixed point
        neam::ct::vector2 get_size() const
        {
          int ret[2];

          glfwGetWindowSize(win, ret, ret + 1);

          return neam::ct::vector2{ret[0], ret[1]};
        }

        /// \brief Return the size of the framebuffer associated with the window
        /// \note the returned vector2 is NOT in fixed point
        neam::ct::vector2 get_framebuffer_size() const
        {
          int ret[2];

          glfwGetFramebufferSize(win, ret, ret + 1);

          return neam::ct::vector2{ret[0], ret[1]};
        }

        /// \brief change the window position
        /// \param window_pos the new coordinates of the window (in pixel). MUST be an integer, NOT a fixed point position.
        void set_position(const neam::ct::vector2 &window_pos)
        {
          glfwSetWindowPos(win, window_pos.x, window_pos.y);
        }

        /// \brief return the position of the window (in screen coordinates -- pixels)
        /// \note the returned vector2 is NOT in fixed point
        neam::ct::vector2 get_position() const
        {
          int ret[2];

          glfwGetWindowPos(win, ret, ret + 1);

          return neam::ct::vector2{ret[0], ret[1]};
        }

        /// \brief change the title of the window
        /// \note This function may only be called from the main thread. (from GLFW documentation)
        void set_title(const std::string &title = "[ neam/yaggler")
        {
          glfwSetWindowTitle(win, title.data());
        }

        /// \brief minimize/iconify the window
        /// \note This function may only be called from the main thread. (from GLFW documentation)
        /// \see restore()
        void iconify() const
        {
          glfwIconifyWindow(win);
        }

        /// \brief hide the window (only for windows in windowed mode)
        /// \note This function may only be called from the main thread. (from GLFW documentation)
        /// \see show()
        void hide() const
        {
          glfwHideWindow(win);
        }

        /// \brief show the window (if already hidden)
        /// \note This function may only be called from the main thread. (from GLFW documentation)
        /// \see hide()
        void show() const
        {
          glfwShowWindow(win);
        }

        /// \brief restore the window if it was previously minimized
        /// \note This function may only be called from the main thread. (from GLFW documentation)
        /// \see iconify()
        void restore() const
        {
          glfwRestoreWindow(win);
        }

        /// \brief check the close flag of the window
        bool should_close() const
        {
          return glfwWindowShouldClose(win);
        }

        /// \brief swap the front and back buffers of the window
        void swap_buffers() const
        {
          glfwSwapBuffers(win);
        }

        /// \brief return the GLFW handle of the current window
        /// \note for an advanced usage
        GLFWwindow *_get_glfw_handle()
        {
          return win;
        }

      private:
        GLFWwindow *win;
        bool link;
    };
  } // namespace yaggler
} // namespace neam

#endif /*__N_15663693851872442693_772963530__YAGGLER_GLFW_WINDOW_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

