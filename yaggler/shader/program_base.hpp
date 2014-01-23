//
// file : program_base.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/program_base.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 19/10/2013 18:04:19
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


#ifndef __N_997627128263917526_441709554__PROGRAM_BASE_HPP__
# define __N_997627128263917526_441709554__PROGRAM_BASE_HPP__

namespace neam
{
  namespace yaggler
  {
    namespace shader
    {
      template<typename ShaderType, typename... Args>
      class program
      {
        // create an error message
        static_assert(!(sizeof...(Args) + 1), "This program type doesn't exist or is not valid.");

        // "disable" this class (to kill even more the user under endless error messages).
        program() = delete;
        ~program() = delete;
      };
    } // namespace shader
  } // namespace yaggler
} // namespace neam

#endif /*__N_997627128263917526_441709554__PROGRAM_BASE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

