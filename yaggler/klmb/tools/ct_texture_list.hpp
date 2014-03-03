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

#include <tools/execute_pack.hpp>
#include <tools/ct_list.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      // a texture list
      template<typename... Textures>
      class texture_list : public ct::type_list_member_instance<Textures...>
      {
        public:
          texture_list()
          {
            // set default texture indexes (in the list order)
            setup_samplers_idxs();
          }
          texture_list(texture_list &&tl) : ct::type_list_member_instance<Textures...>(std::move(static_cast<ct::type_list_member_instance<Textures...> &&>(tl))) {}
          texture_list(const texture_list &tl) : ct::type_list_member_instance<Textures...>(static_cast<const ct::type_list_member_instance<Textures...> &>(tl)) {}

          texture_list(const cr::tuple<Textures...> &o) : ct::type_list_member_instance<Textures...>(o)
          {
            // set default texture indexes (in the list order)
            setup_samplers_idxs();
          }

        private: // use() helpers

          template<size_t... Idxs>
          void _it_use(cr::seq<Idxs...>) const
          {
            NEAM_EXECUTE_PACK((ct::type_list_member_instance<Textures...>::instance.template get<Idxs>().use()));
          }

        private: // setup_samplers_idxs() helpers
          template<size_t... Idxs>
          void _it_setup_samplers_idxs(cr::seq<Idxs...>)
          {
            NEAM_EXECUTE_PACK((ct::type_list_member_instance<Textures...>::instance.template get<Idxs>().set_texture_sampler(Idxs)));
          }

        public:
          // call use() on every textures in the list.
          void use() const
          {
            _it_use(cr::gen_seq<sizeof...(Textures)>());
          }

          // set default texture indexes (in the list order)
          void setup_samplers_idxs()
          {
            _it_setup_samplers_idxs(cr::gen_seq<sizeof...(Textures)>());
          }
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_1662009231574877484_1854883408__CT_TEXTURE_LIST_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

