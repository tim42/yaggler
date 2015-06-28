//
// file : shader_loader.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/shader_loader.hpp
//
// created by : Timothée Feuillet on linux.site
// date: 03/02/2015 12:08:28
//
//
// Copyright (C) 2014 Timothée Feuillet
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

#ifndef __N_1910971541206623151_1304083911__SHADER_LOADER_HPP__
# define __N_1910971541206623151_1304083911__SHADER_LOADER_HPP__

#include "yaggler_type.hpp"

namespace neam
{
  namespace yaggler
  {
    namespace shader
    {
      /// \brief Represent a shader loader interface
      /// It's the interface to be implemented by subsequent classes
      /// there's no need to be constexpr, only static.
      template<typename ShaderLoaderType, typename... Args>
      struct shader_loader
      {
        // create an error message
        static_assert(!(sizeof...(Args) + 1), "This shader loader doesn't exist or is not valid.");

        // "disable" this class (to kill even more the user under endless error messages).
        shader_loader() = delete;
        ~shader_loader() = delete;

        // the interface to be implemented by subsequent classes
        // there's no need to be constexpr, only static.

        /// \brief return true if we need to recompile the shader
        static constexpr bool has_source_changed()
        {
          return false;
        }

        /// \brief return the source code of the shader
        /// \note the return type could be a const char *, char *, std::string, ..
        static constexpr const char *get_source_string()
        {
          return nullptr;
        }

        /// \brief return the source name
        /// \note the return type could be a const char *, char *, std::string, ..
        static constexpr const char *get_source_name()
        {
          return nullptr;
        }
      };
    } // namespace shader
  } // namespace yaggler
} // namespace neam

#endif /*__N_1910971541206623151_1304083911__SHADER_LOADER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on;

