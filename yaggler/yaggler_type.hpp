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

// not enought namespaces ??? :D
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
      namespace opengl
      {
        NYST_CLASS(file);
        NYST_CLASS(constexpr_string);
        NYST_CLASS(dyn_string); // in that case, the 'source' tpl param is the default value.
        NYST_CLASS(function);   // in that case, the 'source' tpl param is a function pointer: std::string (*)(void).
                                // (could be used to inflate the source ;) )

        namespace embed
        {
          using __function_pointer =  std::string (*)();
          N_EMBED_USING(function_ptr, __function_pointer);
        } // namespace embed

      } // namespace opengl
    } // namespace shader
  } // namespace yaggler
} // namespace neam
#undef NYST_CLASS

#endif /*__N_610462899264315823_1451111101__YAGGLER_TYPE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

