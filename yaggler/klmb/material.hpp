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
#include <klmb/tools/make_material_tuple.hpp>
#include <klmb/tools/make_variable_tuple.hpp>
#include <klmb/tools/shader_framework.hpp>

#include <klmb/klmb_context_helper.hpp>

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
      // Variables is a tuple of variable types
      //
      // As most of the K:LMB framework, this is mostly shader oriented.
      template<typename Shaders, typename Textures, typename VarCtx, typename Variables>
      class base_material
      {
        public:

          template<typename... Pairs>
          base_material(Pairs... pairs)
            : textures(), shader_prog(),
              variable_values(internal::variable_filter<typename Pairs::value_t...>(pairs.value...).instance),
              vctx(neam::yaggler::shader::variable_context<neam::yaggler::shader::contexts::none>::create<neam::yaggler::shader::contexts::fixed>
                   (
                     typename internal::make_tuple<Textures, typename Pairs::value_t...>::type(internal::get_type_instance<Textures, Variables, typename Pairs::value_t>::return_instance(textures, variable_values, pairs.value)...),
                     shader_prog.get_uniform_variable(pairs.variable_name)...
                     // Y. A. Y. :D
                   )),
              variable_strings {{pairs.variable_name...}}
          {
            glGetError(); // because shader_prog is not yet linked.

            // setup shader defines
            _setup_klmb_defines();

            // link it :)
            link_shader();
          }

          void use()
          {
            shader_prog.use();
            vctx.use();
            textures.use();
          }

          // ops
          void link_shader()
          {
            shader_prog.link();
            _reset_context_variables();
          }

          // some getters
          template<size_t Index>
          auto get_variable() -> decltype(Variables().template get_ref<Index>())
          {
            return variable_values.template get_ref<Index>();
          }
          template<size_t Index>
          auto get_variable() const -> decltype(Variables().template get<Index>())
          {
            return variable_values.template get<Index>();
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

          // you may want to call this after a program link.
          void _reset_context_variables()
          {
            _set_variables(cr::gen_seq<VarCtx::get_number_of_variables()>());
          }

          // called to reset the klmb shader 'framework'
          void _setup_klmb_defines()
          {
            _klmb_defines_loop_over_shaders(cr::gen_seq<Shaders::size>());
          }

        private:
          // context
          template<size_t... Idxs>
          void _set_variables(neam::cr::seq<Idxs...>)
          {
            vctx.set_variables(shader_prog.get_uniform_variable(variable_strings[Idxs])...);
          }

          // shader framework
          template<size_t Idx>
          char _klmb_defines_single_shader_setup(uint8_t &prog_counter)
          {
            auto &shader = shader_prog.template get_shader_at_index<1>();

            bool is_using_klmb = shader.get_preprocessor_value("KLMB_IS_USING_FRAMEWORK")[0] == '1';

            // setup klmb frmwk
            if (is_using_klmb)
              prog_counter += internal::setup_shader_framework<std::remove_reference<decltype(shader)>::type::shader_type>::setup(shader, prog_counter);

            return 0;
          }

          template<size_t... Idxs>
          void _klmb_defines_loop_over_shaders(neam::cr::seq<Idxs...>)
          {
            uint8_t prog_counter = 0;
            void((char []){_klmb_defines_single_shader_setup<Idxs>(prog_counter)...}); // who knows how this'll be optimised out ?
            // (and which compiler supports it...)
          }

        private:
          Textures textures;
          typename Shaders::template program_t<> shader_prog;

          Variables variable_values;

          VarCtx vctx; // must be last (after the textures, after shader_prog and after the variable values)
          std::array<std::string, VarCtx::get_number_of_variables()> variable_strings;
      };

      namespace internal
      {
        // forwarder for arguments
        template<typename MaterialType, typename VarCtxPairsTuple>
        struct material_creator
        {

        };

        template<typename MaterialType, typename... VarCtxPairs>
        struct material_creator<MaterialType, cr::tuple<VarCtxPairs...>>
        {
          static MaterialType create_base_material( VarCtxPairs && ... vctx)
          {
            return MaterialType(std::move(vctx)...);
          }
        };
      } // namespace internal

      // a nice helper :)
      // (particularly useful for building the variable context...)
      template<typename Shaders, typename Textures, typename... VarCtxPairs>
      base_material<Shaders, Textures, typename internal::template context_t<Textures, VarCtxPairs...>, typename internal::variable_filter<VarCtxPairs...>::tuple> create_base_material(VarCtxPairs... vctx)
      {
        return internal::material_creator<base_material<Shaders, Textures, typename internal::template context_t<Textures, VarCtxPairs...>, typename internal::variable_filter<VarCtxPairs...>::tuple>,
               typename internal::variable_indexer<VarCtxPairs...>::tuple >::create_base_material(std::move(vctx)...);
      }

    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_654936713606318107_1809439057__MATERIAL_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

