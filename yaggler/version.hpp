//
// file : version.hpp
// in : file:///home/tim/projects/yaggler/yaggler/version.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 11/01/2014 22:40:38
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


#ifndef __N_244084821997520240_971927677__VERSION_HPP__
# define __N_244084821997520240_971927677__VERSION_HPP__

#include <tools/ct_string.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace version
    {
      // version idx
      constexpr size_t major = _PROJ_VERSION_MAJOR;
      constexpr size_t minor = _PROJ_VERSION_MINOR;
      constexpr size_t superminor = _PROJ_VERSION_SUPERMINOR;

      constexpr string_t short_string = STRINGIZE(_PROJ_VERSION_MAJOR) "." STRINGIZE(_PROJ_VERSION_MINOR) "." STRINGIZE(_PROJ_VERSION_SUPERMINOR);

      #define YAGGLER_BUILD_TIMESTAMP __TIMESTAMP__
      constexpr string_t string = STRINGIZE(_PROJ_VERSION_MAJOR) "." STRINGIZE(_PROJ_VERSION_MINOR) "." STRINGIZE(_PROJ_VERSION_SUPERMINOR) " (" YAGGLER_BUILD_TIMESTAMP ")";
    } // namespace version
  } // namespace yaggler
} // namespace neam

#endif /*__N_244084821997520240_971927677__VERSION_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

