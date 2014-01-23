//
// file : texture_base.hpp
// in : file:///home/tim/projects/yaggler/yaggler/texture/texture_base.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 11/11/2013 23:25:32
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


#ifndef __N_3286832401173285255_645793128__TEXTURE_BASE_HPP__
# define __N_3286832401173285255_645793128__TEXTURE_BASE_HPP__

namespace neam
{
  namespace yaggler
  {
    namespace texture
    {
      template<typename ObjectType, typename... Args>
      class texture
      {
        static_assert(!(sizeof...(Args) + 1), "This texture type doesn't exist or is not valid.");
      };
    } // namespace texture
  } // namespace yaggler
} // namespace neam

#endif /*__N_3286832401173285255_645793128__TEXTURE_BASE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

