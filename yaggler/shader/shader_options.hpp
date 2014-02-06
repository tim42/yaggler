//
// file : shader_options.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/shader_options.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 17/10/2013 17:01:16
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


#ifndef __N_963786808612749308_1624181515__SHADER_OPTIONS_HPP__
# define __N_963786808612749308_1624181515__SHADER_OPTIONS_HPP__

#include <tools/embed.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace shader
    {
      enum class shader_option
      {
        reload_on_change,       // ask to watch the shader file and recompile it when the file changes
        one_shot_compilation,   // never recompile the shader (except when it's manualu asked)
      };
    } // namespace shader
  } // namespace yaggler

  namespace embed
  {
    namespace shader
    {
      N_EMBED_USING(option, yaggler::shader::shader_option);
    } // namespace shader
  } // namespace embed
} // namespace neam

#endif /*__N_963786808612749308_1624181515__SHADER_OPTIONS_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

