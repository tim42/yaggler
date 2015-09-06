//
// file : object.hpp
// in : file:///home/tim/projects/yaggler/yaggler/geometry/vao_base.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 11/11/2013 17:49:09
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


#ifndef __N_9619264371740662187_1611135426__VAO_BASE_HPP__
# define __N_9619264371740662187_1611135426__VAO_BASE_HPP__

namespace neam
{
  namespace yaggler
  {
    namespace geometry
    {
      /// \brief this class represent a vao.
      template<typename VaoType, typename... Args>
      class vao
      {
        static_assert(!(sizeof...(Args) + 1), "This vao type doesn't exist or is not valid.");
      };
    } // namespace geometry
  } // namespace yaggler
} // namespace neam

#endif /*__N_9619264371740662187_1611135426__VAO_BASE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

