//
// file : shader_type.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/shader_type.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 17/10/2013 03:47:16
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

#ifndef __N_610462899264315823_1451111101__YAGGLER_TYPE_HPP__
# define __N_610462899264315823_1451111101__YAGGLER_TYPE_HPP__

#include <tools/embed.hpp>
#include <tools/ownership.hpp>

/// \file yaggler_type.hpp
/// \brief declare some globaly used types

#define NYST_CLASS(n)   class n{n() = delete; ~n() = delete;}
namespace neam
{
  namespace yaggler
  {
    namespace type
    {
      NYST_CLASS(opengl);
    } // namespace type

    namespace shader
    {
      // Shader types (TODO: move in shader/ folder)
      NYST_CLASS(file);
      NYST_CLASS(constexpr_string);
      NYST_CLASS(dyn_string);

      // Shader Envs (TODO: move in shader/ folder)
      NYST_CLASS(no_env);
      NYST_CLASS(simple_preprocessor);
      NYST_CLASS(advanced_preprocessor);
    } // namespace shader
  } // namespace yaggler

  namespace embed
  {
    N_EMBED_USING(GLenum, GLenum);
  } // namespace embed

} // namespace neam
#undef NYST_CLASS

#endif /*__N_610462899264315823_1451111101__YAGGLER_TYPE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

