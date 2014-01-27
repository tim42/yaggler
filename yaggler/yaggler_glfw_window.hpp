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


#ifndef __N_15663693851872442693_772963530__YAGGLER_GLUT_WINDOW_HPP__
# define __N_15663693851872442693_772963530__YAGGLER_GLUT_WINDOW_HPP__

#include <string>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "ct_point.hpp"
#include "yaggler_except.hpp"
#include "setup.hpp"

namespace neam
{
  namespace yaggler
  {
    namespace window_mode
    {
      class fullscreen_t {} fullscreen;
      class windowed_t {} windowed;
    } // namespace window_mode

    class glfw_window
    {
        glfw_window(const glfw_window &) = delete;
        glfw_window(glfw_window &&) = delete;
        glfw_window &operator = (const glfw_window &) = delete;
        glfw_window &operator = (glfw_window &&) = delete;

      public:
        // window_size MUST be an integer, NOT a fixed point size.
        glfw_window(window_mode::windowed_t, const neam::ct::vector2 &window_size, const std::string &title = "[ neam/yaggler")
          : win(nullptr)
        {
          glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ::opengl_version::gl_major);
          glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ::opengl_version::gl_minor);

          // switch over a constexpr value: no overhead/no generaed branchement code.
          if (::opengl_version::gl_major > 3 || (::opengl_version::gl_major == 3 && ::opengl_version::gl_minor >= 2))
          {
            std::cout << "mierda" << std::endl;
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

          if (!(win = glfwCreateWindow(window_size.x, window_size.y, title.data(), 0, 0)))
            throw glfw_exception("GLFW: glfwCreateWindow call failed");

          select();

          // init glew
          glewExperimental = true;
          GLenum err = glewInit();

          throw_on_glerror<glew_exception>("GLEW INTERNAL ERROR (glewInit): ");
          if (err != GLEW_OK)
          {
            throw glew_exception(reinterpret_cast<const char *>(glewGetErrorString(err)));
          }
        }
        glfw_window(window_mode::fullscreen_t, const std::string &title = "[ neam/yaggler")
          : win(nullptr)
        {
          glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ::opengl_version::gl_major);
          glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ::opengl_version::gl_minor);

          // switch over a constexpr value: no overhead/no generaed branchement code.
          if (::opengl_version::gl_major > 3 || (::opengl_version::gl_major == 3 && ::opengl_version::gl_minor >= 2))
          {
            std::cout << "mierda" << std::endl;
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

          const GLFWvidmode *vmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

          if (!(win = glfwCreateWindow(vmode->width, vmode->height, title.data(), glfwGetPrimaryMonitor(), 0)))
            throw glfw_exception("GLFW: glfwCreateWindow call failed");

          select();

          // init glew
          glewExperimental = true;
          GLenum err = glewInit();

          throw_on_glerror<glew_exception>("GLEW INTERNAL ERROR (glewInit): ");
          if (err != GLEW_OK)
          {
            throw glew_exception(reinterpret_cast<const char *>(glewGetErrorString(err)));
          }
        }

        ~glfw_window()
        {
          if (win)
            glfwDestroyWindow(win);
        }

        void select() const
        {
          glfwMakeContextCurrent(win);
        }

        // NOTE: window_size MUST be an integer, NOT a fixed point size.
        void set_size(const neam::ct::vector2 &window_size)
        {
          glfwSetWindowSize(win, window_size.x, window_size.y);
        }

        neam::ct::vector2 get_size() const
        {
          int ret[2];

          glfwGetWindowSize(win, ret, ret + 1);

          return neam::ct::vector2{ret[0], ret[1]};
        }

        neam::ct::vector2 get_framebuffer_size() const
        {
          int ret[2];

          glfwGetFramebufferSize(win, ret, ret + 1);

          return neam::ct::vector2{ret[0], ret[1]};
        }

        // NOTE: window_pos MUST be an integer, NOT a fixed point position.
        void set_position(const neam::ct::vector2 &window_pos)
        {
          glfwSetWindowPos(win, window_pos.x, window_pos.y);
        }

        neam::ct::vector2 get_position() const
        {
          int ret[2];

          glfwGetWindowPos(win, ret, ret + 1);

          return neam::ct::vector2{ret[0], ret[1]};
        }

        void set_title(const std::string &title = "[ neam/yaggler")
        {
          glfwSetWindowTitle(win, title.data());
        }

        void iconify() const
        {
          glfwIconifyWindow(win);
        }

        void hide() const
        {
          glfwHideWindow(win);
        }

        void show() const
        {
          glfwShowWindow(win);
        }

        void restore() const
        {
          glfwRestoreWindow(win);
        }

        bool should_close() const
        {
          return glfwWindowShouldClose(win);
        }

        void swap_buffers() const
        {
          glfwSwapBuffers(win);
        }

      private:
        GLFWwindow *win;
    };
  } // namespace yaggler
} // namespace neam

#endif /*__N_15663693851872442693_772963530__YAGGLER_GLUT_WINDOW_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

