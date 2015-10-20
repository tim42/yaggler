//
// file : shader_env.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/shader_env.hpp
//
// created by : Timothée Feuillet on linux-vnd3.site
// date: 17/10/2015 00:32:12
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

#ifndef __N_150700112218440071_1586947669__SHADER_ENV_HPP__
# define __N_150700112218440071_1586947669__SHADER_ENV_HPP__

namespace neam
{
    namespace yaggler
  {
    namespace shader
    {
      /// \brief Represent a shader loader interface
      /// It's the interface to be implemented by subsequent classes
      /// there's no need to be constexpr nor static.
      template<typename ShaderEnvType, typename... Args>
      struct shader_env
      {
        // create an error message
        static_assert(!(sizeof...(Args) + 1), "This shader env doesn't exist or is not valid.");
      };
    } // namespace shader
  } // namespace yaggler
} // namespace neam

#endif /*__N_150700112218440071_1586947669__SHADER_ENV_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

