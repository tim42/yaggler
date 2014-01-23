//
// file : shader_base.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/shader.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 17/10/2013 03:36:36
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


#ifndef __N_17161887731745411321_916798385__SHADER_BASE_HPP__
# define __N_17161887731745411321_916798385__SHADER_BASE_HPP__

namespace neam
{
  namespace yaggler
  {
    namespace shader
    {
      template<typename ShaderType, typename... Args>
      class shader
      {
          // create an error message
          static_assert(!(sizeof...(Args) + 1), "This shader type doesn't exist or is not valid.");

          // "disable" this class (to kill even more the user under endless error messages).
          shader() = delete;
          ~shader() = delete;
      };
    } // namespace shader
  } // namespace yaggler
} // namespace neam

// I do like inclusions at the end...

#endif /*__N_17161887731745411321_916798385__SHADER_BASE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 
