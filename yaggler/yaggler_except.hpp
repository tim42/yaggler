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
#include <string>
#include <iostream>
#include <stdexcept>
#include <GLEW/glew.h>

#include <tools/demangle.hpp>
#include <tools/logger/logger.hpp>

/// \file yaggler_except.hpp
/// \brief base exceptions for yaggler

namespace neam
{
  /// \brief An exception class that log exception on throw
  class base_runtime_error : public std::exception
  {
    public:
      base_runtime_error(const std::string &s) noexcept
        : str(s)
      {
#ifndef YAGGLER_NO_MESSAGES
        neam::cr::out.error() << LOGGER_INFO << "\n[EXCEPTION]: " << s << std::endl << std::endl;
#endif
      }
      base_runtime_error(std::string &&s) noexcept
        : str(s)
      {
#ifndef YAGGLER_NO_MESSAGES
        neam::cr::out.error() << LOGGER_INFO << "\n[EXCEPTION]: " << s << std::endl << std::endl;
#endif
      }

      virtual ~base_runtime_error()
      {
      }

      virtual const char *what() const noexcept
      {
        return str.data();
      }

    private:
      std::string str;
  };

  /// \brief a generic runtime error that appends the ExceptionType type name to the string
  template<typename ExceptionType>
  class runtime_error : public base_runtime_error
  {
    public:
      runtime_error(const std::string &s) noexcept : base_runtime_error(neam::demangle<ExceptionType>() + ": " + s)
      {
      }
      runtime_error(std::string &&s) noexcept : base_runtime_error(neam::demangle<ExceptionType>() + ": " + (s))
      {
      }

      virtual ~runtime_error()
      {
      }
  };

  namespace yaggler
  {
    /// \brief throw an ExceptionClass in case of an OpenGL error
    /// ExceptionClass may be an exception defined below.
    template<typename ExceptionClass>
    static inline void throw_on_glerror(const char *header)
    {
      GLint error;
      if ((error = glGetError()) != GL_NO_ERROR)
        throw ExceptionClass(header, false);
    }

    /// \brief an exception related with GLFW
    class glfw_exception : public neam::runtime_error<glfw_exception>
    {
      public:
        explicit glfw_exception(const char *__arg, bool _free_message = false) noexcept
          : neam::runtime_error<glfw_exception>(std::string("yaggler/GLFW: ") + __arg)
        {
          if (_free_message)
            delete []  __arg;
        }

        virtual ~glfw_exception() noexcept
        {
        }

      private:
    };

    /// \brief an exception related with GLEW
    class glew_exception : public neam::runtime_error<glew_exception>
    {
      public:
        explicit glew_exception(const char *__arg, bool _free_message = false) noexcept
      :
        neam::runtime_error<glew_exception>(std::string("yaggler/GLEW: ") + __arg)
        {
          if (_free_message)
            delete []  __arg;
        }

        virtual ~glew_exception() noexcept
        {
        }

      private:
    };

    /// \brief an exception related with openGL
    class opengl_exception : public neam::runtime_error<opengl_exception>
    {
      public:
        explicit opengl_exception(const char *__arg, bool _free_message = false) noexcept
      :
        neam::runtime_error<opengl_exception>(std::string("yaggler/OpenGL: ") + __arg)
        {
          if (_free_message)
            delete []  __arg;
        }

        virtual ~opengl_exception() noexcept
        {
        }

      private:
    };

    /// \brief an exception related with yaggler
    class yaggler_exception : public neam::runtime_error<yaggler_exception>
    {
      public:
        explicit yaggler_exception(const char *__arg, bool _free_message = false) noexcept
        :
        neam::runtime_error<yaggler_exception>(std::string("YagGler: ") + __arg)
        {
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
