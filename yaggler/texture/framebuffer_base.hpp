//
// file : framebuffer_base.hpp
// in : file:///home/tim/projects/yaggler/yaggler/texture/framebuffer_base.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 14/01/2014 16:24:56
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


#ifndef __N_8947367081078777004_1006777425__FRAME_BUFFER_BASE_HPP__
# define __N_8947367081078777004_1006777425__FRAME_BUFFER_BASE_HPP__

namespace neam
{
  namespace yaggler
  {
    namespace texture
    {
      template<typename ObjectType, typename... Args>
      class framebuffer
      {
        static_assert(!(sizeof...(Args) + 1), "This framebuffer type doesn't exist or is not valid.");
      };
    } // namespace texture
  } // namespace yaggler
} // namespace neam

#endif /*__N_8947367081078777004_1006777425__FRAME_BUFFER_BASE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

