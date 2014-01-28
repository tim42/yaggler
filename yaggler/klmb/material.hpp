//
// file : material.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/material.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 27/01/2014 12:06:34
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

#ifndef __N_654936713606318107_1809439057__MATERIAL_HPP__
# define __N_654936713606318107_1809439057__MATERIAL_HPP__

#include <klmb/tools/ct_shader_list.hpp>
#include <klmb/tools/ct_texture_list.hpp>

#include <tools/merge_pack.hpp>

// a simple material class.

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      // this is the very basic KLMB/YägGLer base_material.
      //
      // Shaders is a list of shaders files (types),
      // VarCtx is the type of the variable_context
      // Textures is a list of textures types
      template<typename Shaders, typename VarCtx, typename Textures>
      class base_material
      {
        public:
          base_material()
          {
          }

          void use() const
          {
          }

          // some (advanced) getters
          VarCtx &get_variable_context()
          {
            return vctx;
          }
          const VarCtx &get_variable_context() const
          {
            return vctx;
          }

          // some (very advanced) getters/types
          using _shaders_pack = Shaders;
          using _textures_pack = Textures;

          

        private:
          VarCtx vctx;
          
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_654936713606318107_1809439057__MATERIAL_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

