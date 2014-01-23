//
// file : yaggler_except.hpp
// in : file:///home/tim/projects/yaggler/yaggler/yaggler_except.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 08/10/2013 17:23:42
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


#ifndef __YAGGLER_EXCEPT_HPP__
# define __YAGGLER_EXCEPT_HPP__

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <GLEW/glew.h>
#include "tools/string.hpp"

// base exceptions for yaggler

namespace neam
{
  class runtime_error
  {
    public:
      runtime_error(const neam::cr::string &s) noexcept
        : str(s)
      {
      }
      runtime_error(neam::cr::string &&s) noexcept
        : str(s)
      {
      }

      virtual ~runtime_error()
      {
      }

      const char *what() const
      {
        return str.data();
      }

    private:
      neam::cr::string str;
  };

  namespace yaggler
  {
    // throw an ExceptionClass in case of an OpenGL error
    // ExceptionClass may be an exception defined here.
    template<typename ExceptionClass>
    static inline void throw_on_glerror(const char *header)
    {
      GLint error;
      if ((error = glGetError()) != GL_NO_ERROR)
        throw ExceptionClass(header, false);
    }

    // exception related with GLFW
    class glfw_exception : public neam::runtime_error
    {
      public:
        explicit glfw_exception(const char *__arg, bool _free_message = false) noexcept
          : neam::runtime_error(neam::cr::string("yaggler/GLFW: ") + __arg)
        {
          std::cerr << "yaggler/glfw exception: " << __arg << std::endl;

          if (_free_message)
            delete []  __arg;
        }

        virtual ~glfw_exception() noexcept
        {
        }

      private:
    };

    // exception related with GLEW
    class glew_exception : public neam::runtime_error
    {
      public:
        explicit glew_exception(const char *__arg, bool _free_message = false) noexcept
      :
        neam::runtime_error(neam::cr::string("yaggler/GLEW: ") + __arg)
        {
          std::cerr << "yaggler/glew exception: " << __arg << std::endl;

          if (_free_message)
            delete []  __arg;
        }

        virtual ~glew_exception() noexcept
        {
        }

      private:
    };

    // exception related with GLUT
    class opengl_exception : public neam::runtime_error
    {
      public:
        explicit opengl_exception(const char *__arg, bool _free_message = false) noexcept
      :
        neam::runtime_error(neam::cr::string("yaggler/OpenGL: ") + __arg)
        {
          std::cerr << "yaggler/opengl exception: " << __arg << std::endl;

          if (_free_message)
            delete []  __arg;
        }

        virtual ~opengl_exception() noexcept
        {
        }

      private:
    };

    // exception related with yaggler
    class yaggler_exception : public neam::runtime_error
    {
      public:
        explicit yaggler_exception(const char *__arg, bool _free_message = false) noexcept
        :
        neam::runtime_error(neam::cr::string("YagGler: ") + __arg)
        {
          std::cerr << "yaggler exception: " << __arg << std::endl;

          if (_free_message)
            delete []  __arg;
        }

        virtual ~yaggler_exception() noexcept
        {
        }

      private:
    };
  } // namespace yaggler
} // namespace neam

#endif /*__YAGGLER_EXCEPT_HPP__*/
