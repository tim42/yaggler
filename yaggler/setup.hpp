//
// file : setup.hpp
// in : file:///home/tim/projects/yaggler/yaggler/setup.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 19/10/2013 04:07:49
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


#ifndef __N__SETUP_HPP__
# define __N__SETUP_HPP__

#include <cstddef>

namespace neam
{
  namespace yaggler
  {
    namespace setup
    {
      enum class opengl_profile
      {
        any,
        core,
        compat
      };
      enum class opengl_context_flag
      {
        none,
        forward_compat,
        debug,
      };

      template<size_t GlMajor, size_t GlMinor, opengl_profile Profile = opengl_profile::core, opengl_context_flag CtxFlags = opengl_context_flag::none>
      struct opengl
      {
        static constexpr size_t gl_major = GlMajor;
        static constexpr size_t gl_minor = GlMinor;
        static constexpr opengl_profile profile = Profile;
        static constexpr bool forward_compat = (int)CtxFlags & (int)opengl_context_flag::forward_compat;
        static constexpr bool debug = (int)CtxFlags & (int)opengl_context_flag::debug;
      };
    } // namespace setup
  } // namespace yaggler
} // namespace neam

#endif /*__N__SETUP_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

