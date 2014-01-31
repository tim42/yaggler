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
#include "tools/make_material_tuple.hpp"
#include "klmb_context_helper.hpp"

#include <tools/merge_pack.hpp>
#include <shader/context.hpp>

// a simple material class.

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      // this is the very basic K:LMB/YägGLer base_material.
      //
      // Shaders is a list of shaders files (types),
      // VarCtx is the type of the variable_context
      // Textures is a list of textures types
      //
      // As most of the K:LMB framework, this is mostly shader oriented.
      template<typename Shaders, typename Textures, typename VarCtx>
      class base_material
      {
        public:

          template<typename... Pairs>
          base_material(Pairs... pairs)
            : textures(), shader_prog(),
              vctx(neam::yaggler::shader::variable_context<neam::yaggler::shader::contexts::none>::create<neam::yaggler::shader::contexts::fixed>
                   (
                     typename internal::make_tuple<Textures, typename Pairs::value_t...>::type(internal::get_type_instance<Textures, typename Pairs::value_t>::return_instance(textures, pairs.value)...),
                     shader_prog.get_uniform_variable(pairs.variable_name)... // Y. A. Y. :D
                   ))
          {
            shader_prog.link();
          }

          void use()
          {
            shader_prog.use();
            vctx.use();
            textures.use();
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

          // get the texture tuple
          typename Textures::tuple_type &get_texture_tuple()
          {
            return textures.instance;
          }
          typename Textures::tuple_type &get_texture_tuple() const
          {
            return textures.instance;
          }

          // get a specific texture by its index
          template<size_t Index>
          typename Textures::template get_type<Index> &get_texture()
          {
            return textures.instance.template get_ref<Index>();
          }
          template<size_t Index>
          const typename Textures::template get_type<Index> &get_texture() const
          {
            return textures.instance.template get<Index>();
          }

          // return the shader program
          typename Shaders::template program_t<> &get_shader_program()
          {
            return shader_prog;
          }
          const typename Shaders::template program_t<> &get_shader_program() const
          {
            return shader_prog;
          }

          // some (very advanced) getters/types
          using _shaders_pack = Shaders;
          using _textures_pack = Textures;
          using _var_ctx = VarCtx;

        private:
          Textures textures;
          typename Shaders::template program_t<> shader_prog;
          VarCtx vctx; // must be last
      };

      // a nice helper :)
      // (particularly useful for building the variable context...)
//       template<typename Shaders, typename Textures, typename VarCtx>
//       base_material<Shaders, Textures, VarCtx> create_base_material(const VarCtx &vctx)
//       {
//         return base_material<Shaders, Textures, VarCtx>(vctx);
//       }

      template<typename Shaders, typename Textures, typename... VarCtxPairs>
      base_material<Shaders, Textures, typename internal::template context_t<Textures, VarCtxPairs...>> create_base_material(VarCtxPairs... vctx)
      {
        return base_material<Shaders, Textures, typename internal::template context_t<Textures, VarCtxPairs...>>(vctx...);
      }

    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_654936713606318107_1809439057__MATERIAL_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

