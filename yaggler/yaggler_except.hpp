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
      base_runtime_error(const std::string &s, const std::string &file, size_t line) noexcept
        : str(s)
      {
#ifndef YAGGLER_NO_MESSAGES
        neam::cr::out.error() << LOGGER_INFO_TPL(file, line) << "[EXCEPTION]: " << s << std::endl << std::endl;
#endif
      }
      base_runtime_error(std::string &&s, const std::string &file, size_t line) noexcept
        : str(s)
      {
#ifndef YAGGLER_NO_MESSAGES
        neam::cr::out.error() << LOGGER_INFO_TPL(file, line) << "[EXCEPTION]: " << s << std::endl << std::endl;
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
      runtime_error(const std::string &s, const std::string &file, size_t line) noexcept
        : base_runtime_error(neam::demangle<ExceptionType>() + ": " + s, file, line)
      {
      }
      runtime_error(std::string &&s, const std::string &file, size_t line) noexcept
        : base_runtime_error(neam::demangle<ExceptionType>() + ": " + (s), file, line)
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
    static inline void throw_on_glerror(const std::string &message, const std::string &file, size_t line)
    {
      GLint error;
      std::string s;
      if ((error = glGetError()) != GL_NO_ERROR)
      {
        s = reinterpret_cast<const char *>(glewGetErrorString(error));
        throw ExceptionClass(message + ": " + s, file, line);
      }
    }

    /// \brief an exception related with GLFW
    class glfw_exception : public neam::runtime_error<glfw_exception>
    {
      public:
        explicit glfw_exception(const std::string &str, const std::string &file, size_t line) noexcept
          : neam::runtime_error<glfw_exception>("yaggler/GLFW: " + str, file, line)
        {
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
        explicit glew_exception(const std::string &str, const std::string &file, size_t line) noexcept
          : neam::runtime_error<glew_exception>("yaggler/GLEW: " + str, file, line)
        {
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
        explicit opengl_exception(const std::string &str, const std::string &file, size_t line) noexcept
          : neam::runtime_error<opengl_exception>("yaggler/OpenGL: " + str, file, line)
        {
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
        explicit yaggler_exception(const std::string &str, const std::string &file, size_t line) noexcept
          : neam::runtime_error<yaggler_exception>("YagGler: " + str, file, line)
        {
        }

        virtual ~yaggler_exception() noexcept
        {
        }

      private:
    };
  } // namespace yaggler
} // namespace neam

#endif /*__YAGGLER_EXCEPT_HPP__*/
