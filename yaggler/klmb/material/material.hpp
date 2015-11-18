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
        extern constexpr neam::string_t main_frag = "data/klmb-framework/main.frag";
        extern constexpr neam::string_t main_vert = "data/klmb-framework/main.vert";
        extern constexpr neam::string_t main_geom = "data/klmb-framework/main.geom";
      } // namespace framework
      /// \brief The default framework files
      using default_framework_files = ct::type_list
      <
        // Framework files
        ct::pair<embed::GLenum<GL_VERTEX_SHADER>, auto_file_shader<framework_files::main_vert>>,
        ct::pair<embed::GLenum<GL_GEOMETRY_SHADER>, auto_file_shader<framework_files::main_geom>>,
        ct::pair<embed::GLenum<GL_FRAGMENT_SHADER>, auto_file_shader<framework_files::main_frag>>
      >;

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
      /// \p FrameworkShaders A neam::ct::type_list of neam::ct::pair< embed::GLenum< GL_*_SHADER >, whatever_shader_type >
      ///    This parameter has a default value and should only be set when there's an absolute need of that
      ///    (like, for example, changing the default color merging algo to implement a controlled color grading).
      ///    If overridden by custom shaders, you should follow *exactly* how default shaders are implemented,
      ///    and you have to specify a shader for each stage with at least two shader entry points
      ///    The default value is default_framework_files
      ///
      /// \see default_framework_files
      ///
      /// \p Framework The framework to use for this material
      /// \note the shader could specify the framework they are using and their version by defining those macros:
      ///       \code FRAMEWORK [framework-name]\endcode (default framework name is K:LMB)
      ///       \code FRAMEWORK_VERSION [framework-version]\endcode
      ///       If the version or the name doesn't match an error is triggered (with a proper message in the console)
      ///
      /// \see default_shader_framework
      ///
      /// \note A shader used by this class MUST define the following macro: \code KLMB_IS_USING_FRAMEWORK \endcode, independently of the framework used
      ///
      /// The default framework (K:LMB) ask the shaders to define the following marco (in some cases):
      ///   \code KLMB_IS_ENTRY_POINT \endcode : Define it if the shader file has a main function (under K:LMB framework \e KLMB_MAIN_FUNCTION)
      ///   \code KLMB_NUMBER_OF_OUTPUT_BUFFER [the actual number] \endcode The default is 1. It is the number of output buffers the shader write.
      ///         Each output buffer have a pre-defined output variable: \code KLMB_SHARED_NAME(color_[output-buffer-index]) \endcode
      /// Under the K:LMB framework, you could define multiple shaders with an entry point, but only in the Fragment shader stage
      /// K:LMB files are located in \e data/klmb-framework
      ///
      /// As most of the K:LMB framework, this is mostly shader oriented.
      template<typename Shaders, typename Textures, typename VarCtx, typename Variables, typename FrameworkShaders = default_framework_files, typename Framework = default_shader_framework>
      class base_material
      {
        private:
          static constexpr GLuint __init_vars(const std::string &)
          {
            return 0;
          }

        public:
          /// \brief the shader program type
          using program_t = typename Shaders::template program_auto_merger<FrameworkShaders>::type;

        public:
          /// \brief Initialize the material
          /// \note You should not use this directly.
          /// \see create_material()
          /// \see create_material_ptr()
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
            glGetError(); // because shader_prog is not yet linked, we remove the error

            // setup shader defines
            _setup_klmb_defines();

            // link it :)
            link_shader();
          }

          /// \brief Moves a shader
          /// \warning if I remember things correctly, indexed_variables won't work here, as they will keep their references to the old \p mt variables<>
          /// \todo fix the indexed_variables issue
          base_material(base_material &&mt)
            : textures(std::move(mt.textures)), shader_prog(std::move(mt.shader_prog)),
              variable_values(std::move(mt.variable_values)),
              vctx(std::move(mt.vctx)), variable_strings(std::move(mt.variable_strings)), variable_buffers(std::move(mt.variable_buffers))
          {
          }

          /// \brief bind the material for use
          void use()
          {
            shader_prog.use();
            vctx.use();
            textures.use();
          }

          /// \brief link the shader and retrieve the uniform variables from it
          void link_shader()
          {
            shader_prog.link();
            _reset_context_variables();
          }

          /// \brief retrieve a previously declared variable<> by its index
          /// \note as it return a reference, you could do \code material.get_variable<0>() = 42.f; \endcode
          template<size_t Index>
          auto get_variable() ->  decltype(Variables().template get<Index>())
          {
            return variable_values.template get<Index>();
          }

          /// \brief retrieve a previously declared variable<> by its index
          /// \note it returns a const reference
          template<size_t Index>
          auto get_variable() const -> decltype(Variables().template get<Index>())
          {
            return variable_values.template get<Index>();
          }

          /// \brief returns the variable context
          /// \see yaggler::shader::variable_context<>
          /// \note intended for advanced usage only
          VarCtx &get_variable_context()
          {
            return vctx;
          }

          /// \brief returns the variable context
          /// \see yaggler::shader::variable_context<>
          /// \note intended for advanced usage only
          const VarCtx &get_variable_context() const
          {
            return vctx;
          }

          /// \brief get the texture tuple
          typename Textures::tuple_type &get_texture_tuple()
          {
            return textures.instance;
          }

          /// \brief get the texture tuple
          typename Textures::tuple_type &get_texture_tuple() const
          {
            return textures.instance;
          }

          /// \brief get a specific texture by its index
          template<size_t Index>
          typename Textures::template get_type<Index> &get_texture()
          {
            return textures.instance.template get<Index>();
          }

          /// \brief get a specific texture by its index
          template<size_t Index>
          const typename Textures::template get_type<Index> &get_texture() const
          {
            return textures.instance.template get<Index>();
          }

          /// \brief return the shader program
          program_t &get_shader_program()
          {
            return shader_prog;
          }

          /// \brief return the shader program
          const program_t &get_shader_program() const
          {
            return shader_prog;
          }

          using _shaders_pack = Shaders;
          using _textures_pack = Textures;
          using _var_ctx = VarCtx;

          /// \brief you may want to call this after a program link.
          /// \note automatically done after link_shader()
          /// \note intended for advanced usage only
          void _reset_context_variables()
          {
            _set_variables(cr::gen_seq<VarCtx::get_number_of_variables()>());
          }

          /// \brief called to reset the klmb shader 'framework'
          /// \note intended for advanced usage only
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

            bool is_using_klmb = tools::is_true(shader.environment.get_preprocessor_value("KLMB_IS_USING_FRAMEWORK"));

            std::string framework = shader.environment.get_preprocessor_value("FRAMEWORK");
            bool bad_framework = false;
            if (!framework.empty())
            {
              // We have some custom framework
              if (framework != Framework::framework_name)
              {
                neam::cr::out.error() << LOGGER_INFO << "K:LMB/YAGGLER: material: shader '" << shader.get_source_name() << "':" << cr::newline
                                      << "  Shader is expecting framework '" << framework << "' but current framework is '" << Framework::framework_name << "'" << std::endl;
                bad_framework = true;
              }
            }

            std::string framework_version = shader.environment.get_preprocessor_value("FRAMEWORK_VERSION");
            if (!framework_version.empty())
            {
              // TODO: maybe a better framework version comparison
              if (framework_version != Framework::framework_version)
              {
                neam::cr::out.error() << LOGGER_INFO << "K:LMB/YAGGLER: material: shader '" << shader.get_source_name() << "':" << cr::newline
                                      << "  Shader is expecting framework version '" << framework_version << "' but current framework version is '" << Framework::framework_version << "'" << cr::newline
                                      << "  Framework name: '" << Framework::framework_name << "'" << std::endl;
                bad_framework = true;
              }
            }

            // setup klmb frmwk
            if (is_using_klmb && !bad_framework)
              prog_counter += Framework::template setup_framework<std::remove_reference<decltype(shader)>::type::shader_type>::setup(shader, prog_counter, framework_data);
#ifndef YAGGLER_NO_MESSAGES
            else if (shader.shader_type == GL_FRAGMENT_SHADER || shader.shader_type == GL_VERTEX_SHADER)
              neam::cr::out.warning() << LOGGER_INFO << "K:LMB/YAGGLER: material: shader framework: shader '" << shader.get_source_name() << "' is not using " << Framework::framework_name << " framework." << std::endl;
#endif

            return 0;
          }

          template<size_t... Idxs>
          void _klmb_defines_loop_over_shaders(neam::cr::seq<Idxs...>)
          {
            uint8_t prog_counter = 0;
            typename Framework::framework_data framework_data;

            // setup number of shaders
            framework_data.fragment_shader_number = Shaders::fragment_shaders_t::size();
            framework_data.geometry_shader_number = Shaders::geometry_shaders_t::size();
            framework_data.tes_shader_number = Shaders::tess_evaluation_shaders_t::size();
            framework_data.tcs_shader_number = Shaders::tess_control_shaders_t::size();
            framework_data.vertex_shader_number = Shaders::vertex_shaders_t::size();
            framework_data.compute_shader_number = Shaders::compute_shaders_t::size();

            NEAM_EXECUTE_PACK((_klmb_defines_single_shader_setup<Idxs>(prog_counter, framework_data)));
          }

        private:
          Textures textures;

          // framework shader files

          // the prog
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

      /// \brief an helper for creating raw materials (there's nothing more than you use/defines)
      /// this particularly useful for building the variable context...
      /// \note A convention exists where the first variable should be the vp_matrix and the second one the object_matrix
      /// \see create_material()
      /// \see variables_indexes::variable_indexes
      template<typename Shaders, typename Textures, typename... VarCtxPairs>
      base_material<Shaders, Textures, typename internal::template context_t<Textures, VarCtxPairs...>, typename internal::variable_filter<VarCtxPairs...>::tuple>
      create_base_material(VarCtxPairs... vctx)
      {
        return internal::material_creator
        <
          base_material
          <
            Shaders,
            Textures,
            typename internal::template context_t<Textures, VarCtxPairs...>,
            typename internal::variable_filter<VarCtxPairs...>::tuple
          >,
          typename internal::variable_indexer<VarCtxPairs...>::tuple
        >::create_base_material(std::move(vctx)...);
      }

      /// \brief an helper for creating raw materials (there's nothing more than you use/defines)
      /// this particularly useful for building the variable context...
      /// \note A convention exists where the first variable should be the vp_matrix and the second one the object_matrix
      /// \see create_material()
      /// \see variables_indexes::variable_indexes
      template<typename Shaders, typename Textures, typename FrameworkShaders, typename... VarCtxPairs, typename Framework = default_shader_framework>
      base_material<Shaders, Textures, typename internal::template context_t<Textures, VarCtxPairs...>, typename internal::variable_filter<VarCtxPairs...>::tuple, FrameworkShaders, Framework>
      create_base_material(VarCtxPairs... vctx)
      {
        return internal::material_creator
        <
          base_material
          <
            Shaders,
            Textures,
            typename internal::template context_t<Textures, VarCtxPairs...>,
            typename internal::variable_filter<VarCtxPairs...>::tuple,
            FrameworkShaders,
            Framework
          >,
          typename internal::variable_indexer<VarCtxPairs...>::tuple
        >::create_base_material(std::move(vctx)...);
      }

      /// \brief an helper for creating raw materials (there's nothing more than you use/defines)
      /// this particularly useful for building the variable context...
      /// \note A convention exists where the first variable should be the vp_matrix and the second one the object_matrix
      /// \see create_material_ptr()
      /// \see variables_indexes::variable_indexes
      template<typename Shaders, typename Textures, typename... VarCtxPairs>
      base_material<Shaders, Textures, typename internal::template context_t<Textures, VarCtxPairs...>, typename internal::variable_filter<VarCtxPairs...>::tuple> *
      create_base_material_ptr(VarCtxPairs... vctx)
      {
        return internal::material_creator
        <
          base_material
          <
            Shaders,
            Textures,
            typename internal::template context_t<Textures, VarCtxPairs...>,
            typename internal::variable_filter<VarCtxPairs...>::tuple
          >,
          typename internal::variable_indexer<VarCtxPairs...>::tuple
        >::create_base_material_ptr(std::move(vctx)...);
      }

      /// \brief an helper for creating raw materials (there's nothing more than you use/defines)
      /// this particularly useful for building the variable context...
      /// \note A convention exists where the first variable should be the vp_matrix and the second one the object_matrix
      /// \see create_material_ptr()
      /// \see variables_indexes::variable_indexes
      template<typename Shaders, typename Textures, typename FrameworkShaders, typename... VarCtxPairs, typename Framework = default_shader_framework>
      base_material<Shaders, Textures, typename internal::template context_t<Textures, VarCtxPairs...>, typename internal::variable_filter<VarCtxPairs...>::tuple, FrameworkShaders, Framework> *
      create_base_material_ptr(VarCtxPairs... vctx)
      {
        return internal::material_creator
        <
          base_material
          <
            Shaders,
            Textures,
            typename internal::template context_t<Textures, VarCtxPairs...>,
            typename internal::variable_filter<VarCtxPairs...>::tuple,
            FrameworkShaders,
            Framework
          >,
          typename internal::variable_indexer<VarCtxPairs...>::tuple
        >::create_base_material_ptr(std::move(vctx)...);
      }

      /// \brief an helper for creating materials
      /// It adds references to the vp_matrix and the object_matrix automatically (as the first and second variables)
      template<typename Shaders, typename Textures, typename... VarCtxPairs>
      auto create_material(VarCtxPairs... vctx)
      {
        return create_base_material<Shaders, Textures>
               (
                 neam::klmb::yaggler::make_ctx_pair("vp_matrix", neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)), // allow camera switches
                 neam::klmb::yaggler::make_ctx_pair("object_matrix", neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)),
                 std::move(vctx)...
               );
      }

      /// \brief an helper for creating materials
      /// It adds references to the vp_matrix and the object_matrix automatically (as the first and second variables)
      template<typename Shaders, typename Textures, typename FrameworkShaders, typename... VarCtxPairs, typename Framework = default_shader_framework>
      auto create_material(VarCtxPairs... vctx)
      {
        return create_base_material<Shaders, Textures, FrameworkShaders, Framework>
               (
                 neam::klmb::yaggler::make_ctx_pair("vp_matrix", neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)), // allow camera switches
                 neam::klmb::yaggler::make_ctx_pair("object_matrix", neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)),
                 std::move(vctx)...
               );
      }

      /// \brief an helper for creating materials
      /// It adds references to the vp_matrix and the object_matrix automatically (as the first and second variables)
      template<typename Shaders, typename Textures, typename... VarCtxPairs>
      auto create_material_ptr(VarCtxPairs... vctx)
      {
        return create_base_material_ptr<Shaders, Textures>
               (
                 neam::klmb::yaggler::make_ctx_pair("vp_matrix", neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)), // allow camera switches
                 neam::klmb::yaggler::make_ctx_pair("object_matrix", neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)),
                 std::move(vctx)...
               );
      }

      /// \brief an helper for creating materials
      /// It adds references to the vp_matrix and the object_matrix automatically (as the first and second variables)
      template<typename Shaders, typename Textures, typename FrameworkShaders, typename... VarCtxPairs, typename Framework = default_shader_framework>
      auto create_material_ptr(VarCtxPairs... vctx)
      {
        return create_base_material_ptr<Shaders, Textures, FrameworkShaders, Framework>
               (
                 neam::klmb::yaggler::make_ctx_pair("vp_matrix", neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)), // allow camera switches
                 neam::klmb::yaggler::make_ctx_pair("object_matrix", neam::klmb::yaggler::variable<glm::mat4 *>(nullptr)),
                 std::move(vctx)...
               );
      }


      namespace variable_indexes
      {
        /// \brief holds default, conventional indexes for some most used variable indexes
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

