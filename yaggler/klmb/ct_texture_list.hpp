//
// file : ct_texture_list.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/ct_texture_list.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 27/01/2014 20:38:38
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

#ifndef __N_1662009231574877484_1854883408__CT_TEXTURE_LIST_HPP__
# define __N_1662009231574877484_1854883408__CT_TEXTURE_LIST_HPP__

#include <tools/ct_list.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      // a texture list
      template<typename... Textures>
      struct texture_type_list : public ct::type_list_member_instance<Textures...>
      {
        texture_type_list() {}
        texture_type_list(const texture_type_list &o) : ct::type_list_member_instance<Textures...>(o) {}
        texture_type_list(const cr::tuple<Textures...> &o) : ct::type_list_member_instance<Textures...>(o) {}

        template<typename... Vals>
        texture_type_list(Vals... vals) : ct::type_list_member_instance<Textures...>(std::move(vals)...) {}
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_1662009231574877484_1854883408__CT_TEXTURE_LIST_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

