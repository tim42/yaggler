//
// file : ct_list.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/ct_list.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 27/01/2014 16:26:19
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

#ifndef __N_48888409651922371_1596013433__CT_LIST_HPP__
# define __N_48888409651922371_1596013433__CT_LIST_HPP__

#include <tools/type_at_index.hpp>
#include <tools/tuple.hpp>

namespace neam
{
  namespace ct
  {
    // a list of types
    template<typename... Types>
    struct type_list
    {
      template<size_t Index>
      using get_type = typename type_at_index<Index, Types...>::type;

      template<typename... Values>
      static constexpr cr::tuple<Types...> instanciate_tuple(Values... vals)
      {
        return cr::tuple<Types...>(std::move(vals)...);
      }

      template<size_t Index, typename... Values>
      static typename type_at_index<Index, Types...>::type instanciate_one(Values... vals)
      {
        return type_at_index<Index, Types...>::type(std::move(vals)...);
      };
    };

    // a list of types, with a static instance of the tuple
    template<typename... Types>
    struct type_list_static_instance : public type_list<Types...>
    {
      static cr::tuple<Types...> instance;
    };

    template<typename... Types>
    cr::tuple<Types...> type_list_static_instance<Types...>::instance;

    // a list of types, with a non-static instance of the tuple
    template<typename... Types>
    struct type_list_member_instance : public type_list<Types...>
    {
      type_list_member_instance() : cr::tuple<Types...>() {}
      type_list_member_instance(const type_list_member_instance &o) : cr::tuple<Types...>(o.instance) {}
      type_list_member_instance(const cr::tuple<Types...> &o) : cr::tuple<Types...>(o) {}

      template<typename... Vals>
      type_list_member_instance(Vals... vals) : cr::tuple<Types...>(vals...) {}


      cr::tuple<Types...> instance;
    };
  } // namespace ct
} // namespace neam

#endif /*__N_48888409651922371_1596013433__CT_LIST_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

