//
// file : yagller_init.hpp
// in : file:///home/tim/projects/yaggler/yaggler/yagller_init.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 08/10/2013 17:06:41
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

#ifndef __N_10073651771905376768_1961537927__YAGLLER_INIT_HPP__
# define __N_10073651771905376768_1961537927__YAGLLER_INIT_HPP__

#include <iostream>
#include <cstdio>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

// #include <GL/gl.h>
#include <tools/logger/logger.hpp>

#include "yaggler_except.hpp"
#include "ct_point.hpp"

#ifndef YAGGLER_NO_MESSAGES
#include "version.hpp"
#endif

namespace neam
{
  namespace yaggler
  {
    // this class perform the basic glfw init (it do not create the window)
    // it is possible to do calls to glfw function outside this class.
    // this is just an helper ;)
    class yaggler_init
    {
      public:
        yaggler_init()
        {
#ifndef YAGGLER_NO_MESSAGES
          neam::cr::out.info() << LOGGER_INFO << "yaggler version " << version::string << std::endl;
#endif

          // init callbacks on error/warning
          glfwSetErrorCallback([](int code, const char *message)
          {
            (void)code;
#ifndef YAGGLER_NO_MESSAGES
            if (::opengl_version::debug)
              neam::cr::out.warning() << LOGGER_INFO << "GLFW error [" << code << "]: '" << message << "'" << std::endl;
#endif
            throw glfw_exception(message, false);
          });

          // init glfw
          if (!glfwInit())
            throw glfw_exception("unable to init GLFW (glfwInit call failed)");
        }

        ~yaggler_init()
        {
          glfwTerminate();
        }

        // a bunch of deleted constructors/operators
        yaggler_init(const yaggler_init &) = delete;
        yaggler_init(yaggler_init &&) = delete;
        yaggler_init &operator = (const yaggler_init &) = delete;
        yaggler_init &operator = (yaggler_init &&) = delete;
    };
  } // namespace yaggler
} // namespace neam

#endif /*__N_10073651771905376768_1961537927__YAGLLER_INIT_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

