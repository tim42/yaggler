//
// file : compiler_fixs.hpp
// in : file:///home/tim/projects/yaggler/yaggler/compiler_fixs.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 24/01/2014 16:02:59
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

#ifndef __N_237330644282348131_188192629__COMPILER_FIXS_HPP__
# define __N_237330644282348131_188192629__COMPILER_FIXS_HPP__

#ifdef __clang__

# define NY_CLANG_CT_DATA_FIX__GET_DATA         _y_cl__get_data()

# define NY_CLANG_CT_DATA_FIX__DATA_GETTER      constexpr static data_t _y_cl__get_data() { return Data::value; }
# define NY_CLANG_DATA_FIX__DATA_GETTER         data_t _y_cl__get_data() { return Data::value; }

# define NY_CLANG_CT_DATA_FIX__DATA_GETTER_CONST(v)     constexpr static data_t _y_cl__get_data() { return v; }
# define NY_CLANG_DATA_FIX__DATA_GETTER_CONST(v)        data_t _y_cl__get_data() const { return v; }

#else // because G++ works correctly

# define NY_CLANG_CT_DATA_FIX__GET_DATA         data

# define NY_CLANG_CT_DATA_FIX__DATA_GETTER
# define NY_CLANG_DATA_FIX__DATA_GETTER

# define NY_CLANG_CT_DATA_FIX__DATA_GETTER_CONST(v)
# define NY_CLANG_DATA_FIX__DATA_GETTER_CONST(v)

#endif

#endif /*__N_237330644282348131_188192629__COMPILER_FIXS_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

