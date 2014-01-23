//
// file : function.hpp
// in : file:///home/tim/projects/yaggler/yaggler/tools/function.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 16/12/2013 14:10:40
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


#ifndef __N_48585258136941080_735723374__FUNCTION_HPP__
# define __N_48585258136941080_735723374__FUNCTION_HPP__

namespace neam
{
  namespace cr
  {
    template<typename Ptr>
    struct is_function_pointer
    {
      static constexpr bool value = false;
    };
    template<typename Ret, typename... Params>
    struct is_function_pointer<Ret(*)(Params...)>
    {
      static constexpr bool value = true;
    };
    template<typename Ret, typename... Params>
    struct is_function_pointer<Ret(*)(Params......)>
    {
      static constexpr bool value = true;
    };
    template<typename Ret, typename... Params>
    struct is_function_pointer<Ret(&)(Params...)>
    {
      static constexpr bool value = true;
    };
    template<typename Ret, typename... Params>
    struct is_function_pointer<Ret(&)(Params......)>
    {
      static constexpr bool value = true;
    };
  } // namespace cr
} // namespace neam

#endif /*__N_48585258136941080_735723374__FUNCTION_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

