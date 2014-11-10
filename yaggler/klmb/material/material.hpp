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

#include <klmb/material/klmb_context_helper.hpp>
#include <klmb/material/material_usings.hpp>

#include <tools/execute_pack.hpp>
#include <tools/ct_string.hpp>
#include <tools/merge_pack.hpp>

#include <shader/context.hpp>

// a simple material class.

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      namespace framework_files
      {
        constexpr neam::string_t main_frag = "data/klmb-framework/main.frag";
        constexpr neam::string_t main_vert = "data/klmb-framework/main.vert";
        constexpr neam::string_t main_geom = "data/klmb-framework/main.geom";
      } // namespace framework

      /// \brief this is the very basic K:LMB/YägGLer base_material.
      /// you should take a look at the files material_usings.hpp and klmb_context_helper.hpp
      ///
      /// \p Shaders is a list of shaders files (types),
      /// \p VarCtx is the type of the variable_context
      /// \p Textures is a list of textures types
      /// \p Variables is a tuple of variable types (see \file klmb_context_helper.hpp)
      ///    It consist of a tuple of \c material_ctx_pair (\see make_ctx_pair).
      ///    You can reference textures from the texture list by using a \c reference_to_texture as value.
      ///    You can also create 'indexed' variables that could be changed latter via get_variable()
      ///    (a good example is create_material that add two indexed variable that binds
      ///    to "vp_matrix" and "object_matrix" uniforms and are glm::mat4 pointers)
      ///
      /// \see create_base_material
      /// \see create_material
      ///
      /// As most of the K:LMB framework, this is mostly shader oriented.
      template<typename Shaders, typename Textures, typename VarCtx, typename Variables>
      class base_material
      {
        private:
          static constexpr GLuint __init_vars(const std::string &)
          {
            return 0;
          }

        public:
          using program_t = typename Shaders::template program_auto_merger
          <
            ct::pair<embed::GLenum<GL_VERTEX_SHADER>, auto_file_shader<framework_files::main_vert>>,
            ct::pair<embed::GLenum<GL_GEOMETRY_SHADER>, auto_file_shader<framework_files::main_geom>>,
            ct::pair<embed::GLenum<GL_FRAGMENT_SHADER>, auto_file_shader<framework_files::main_frag>>
          >::type;


        public:
          template<typename... Pairs>
          base_material(Pairs... pairs)
            : textures(), shader_prog(),
              variable_values(internal::variable_filter<typename Pairs::value_t...>(pairs.value...).instance),
              vctx(neam::yaggler::shader::variable_context<neam::yaggler::shader::contexts::none>::create<neam::yaggler::shader::contexts::fixed>
                   (
                     typename internal::make_tuple<Textures, typename Pairs::value_t...>::type(internal::get_type_instance<Textures, Variables, typename Pairs::value_t>::return_instance(textures, variable_values, pairs.value)...),
                    __init_vars(pairs.variable_name)...
                     // Y. A. Y. :D
                   )),
              variable_strings {{pairs.variable_name...}},
              variable_buffers {{pairs.buffer_storage...}}
          {
            glGetError(); // because shader_prog is not yet linked.

            // setup shader defines
            _setup_klmb_defines();

            // link it :)
            link_shader();
          }

          base_material(base_material &&mt)
            : textures(std::move(mt.textures)), shader_prog(std::move(mt.shader_prog)),
              variable_values(std::move(mt.variable_values)),
              vctx(std::move(mt.vctx)), variable_strings(std::move(mt.variable_strings)), variable_buffers(std::move(mt.variable_buffers))
          {
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
          auto get_variable() ->  decltype(Variables().template get<Index>())
          {
            return variable_values.template get<Index>();
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
            return textures.instance.template get<Index>();
          }
          template<size_t Index>
          const typename Textures::template get_type<Index> &get_texture() const
          {
            return textures.instance.template get<Index>();
          }

          // return the shader program
          program_t &get_shader_program()
          {
            return shader_prog;
          }
          const program_t &get_shader_program() const
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
            _klmb_defines_loop_over_shaders(cr::gen_seq<program_t::get_ct_shaders_number()>());
          }

        private:
          // context
          template<size_t... Idxs>
          void _set_variables(neam::cr::seq<Idxs...>)
          {
            vctx.set_variables(shader_prog.get_uniform(variable_strings[Idxs], variable_buffers[Idxs])...);
          }

          // shader framework
          template<size_t Idx>
          char _klmb_defines_single_shader_setup(uint8_t &prog_counter, internal::_shader_framework_data &framework_data)
          {
            auto &shader = shader_prog.template get_shader_at_index<Idx>();

            shader._preload();

            bool is_using_klmb = tools::is_true(shader.get_preprocessor_value("KLMB_IS_USING_FRAMEWORK"));

            // setup klmb frmwk
            if (is_using_klmb)
              prog_counter += internal::setup_shader_framework<std::remove_reference<decltype(shader)>::type::shader_type>::setup(shader, prog_counter, framework_data);
#ifndef YAGGLER_NO_MESSAGES
            else if (shader.shader_type == GL_FRAGMENT_SHADER || shader.shader_type == GL_VERTEX_SHADER)
              std::cerr << "K:LMB/YAGGLER: WARNING: material: shader framework: shader '" << shader.get_source_name() << "' is not using K:LMB framework." << std::endl;
#endif

            return 0;
          }

          template<size_t... Idxs>
          void _klmb_defines_loop_over_shaders(neam::cr::seq<Idxs...>)
          {
            uint8_t prog_counter = 0;
            internal::_shader_framework_data framework_data;

            // setup number of shaders
            framework_data.fragment_shader_number = Shaders::fragment_shaders_t::size();
            framework_data.geometry_shader_number = Shaders::geometry_shaders_t::size();
            framework_data.tess_evaluation_shader_number = Shaders::tess_evaluation_shaders_t::size();
            framework_data.tess_control_shader_number = Shaders::tess_control_shaders_t::size();
            framework_data.vertex_shader_number = Shaders::vertex_shaders_t::size();
            framework_data.compute_shader_number = Shaders::compute_shaders_t::size();

            NEAM_EXECUTE_PACK((_klmb_defines_single_shader_setup<Idxs>(prog_counter, framework_data)));
          }

        private:
          Textures textures;

          // framework shader files

          // the prog :)
          program_t shader_prog;

          Variables variable_values;

          VarCtx vctx; // must be last (after the textures, after shader_prog and after the variable values)
          std::array<std::string, VarCtx::get_number_of_variables()> variable_strings;
          std::array<bool, VarCtx::get_number_of_variables()> variable_buffers; // is an uniform buffer ?
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
          static MaterialType create_base_material(VarCtxPairs && ... vctx)
          {
            return MaterialType(std::move(vctx)...);
          }
          static MaterialType *create_base_material_ptr(VarCtxPairs && ... vctx)
          {
            return new MaterialType(std::move(vctx)...);
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

      template<typename Shaders, typename Textures, typename... VarCtxPairs>
      base_material<Shaders, Textures, typename internal::template context_t<Textures, VarCtxPairs...>, typename internal::variable_filter<VarCtxPairs...>::tuple> *create_base_material_ptr(VarCtxPairs... vctx)
      {
        return internal::material_creator<base_material<Shaders, Textures, typename internal::template context_t<Textures, VarCtxPairs...>, typename internal::variable_filter<VarCtxPairs...>::tuple>,
        typename internal::variable_indexer<VarCtxPairs...>::tuple >::create_base_material_ptr(std::move(vctx)...);
      }

      // C++ 1y anyone ?? :)
      template<typename Shaders, typename Textures, typename... VarCtxPairs>
      auto create_material(VarCtxPairs... vctx)
      -> decltype(create_base_material<Shaders, Textures>
                  (
                    neam::klmb::yaggler::make_ctx_pair(nullptr, neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)), // allow camera switchs
                    neam::klmb::yaggler::make_ctx_pair(nullptr, neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)),
                    std::move(vctx)...
                  ))
      {
        return create_base_material<Shaders, Textures>
               (
                 neam::klmb::yaggler::make_ctx_pair("vp_matrix", neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)), // allow camera switchs
                 neam::klmb::yaggler::make_ctx_pair("object_matrix", neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)),
                 std::move(vctx)...
               );
      }
      // C++ 1y anyone ?? :)
      template<typename Shaders, typename Textures, typename... VarCtxPairs>
      auto create_material_ptr(VarCtxPairs... vctx)
      -> decltype(create_base_material_ptr<Shaders, Textures>
                  (
                    neam::klmb::yaggler::make_ctx_pair(nullptr, neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)), // allow camera switchs
                    neam::klmb::yaggler::make_ctx_pair(nullptr, neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)),
                    std::move(vctx)...
                  ))
      {
        return create_base_material_ptr<Shaders, Textures>
               (
                 neam::klmb::yaggler::make_ctx_pair("vp_matrix", neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)), // allow camera switchs
                 neam::klmb::yaggler::make_ctx_pair("object_matrix", neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)),
                 std::move(vctx)...
               );
      }


      namespace variable_indexes
      {
        enum variable_indexes
        {
          vp_matrix = 0,
          object_matrix = 1,

          // last
          count = 2
        };
      } // namespace variable_indexes

    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_654936713606318107_1809439057__MATERIAL_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

