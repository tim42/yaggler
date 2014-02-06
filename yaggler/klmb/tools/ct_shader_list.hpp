//
// file : ct_list.hpp (2)
// in : file:///home/tim/projects/yaggler/yaggler/klmb/ct_list.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 27/01/2014 18:44:13
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

#ifndef __N_18450143281336995145_1567800396__CT_LIST_HPP__2___
# define __N_18450143281336995145_1567800396__CT_LIST_HPP__2___

#include <GLEW/glew.h>
#include <tools/ct_list.hpp>
#include <shader/opengl_program.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      // a shader list
      template<typename... Shaders>
      struct shader_list : public ct::type_list_member_instance<Shaders...>
      {
        private: // helpers
          // Thanks to : stackoverflow.com/q/18366398/
          template <typename Cr, typename Type> struct make_type;

          template <typename Cr, typename ...Others>
          struct make_type<Cr, cr::tuple<Others...>>
          {
            using type = cr::tuple<Cr, Others...>;
          };

          template<GLenum ShaderType, typename...>
          struct filter
          {
            using type = cr::tuple<>;
          };
          template<GLenum ShaderType, typename Current, typename... Types>
          struct filter<ShaderType, Current, Types...>
          {
            using type = typename std::conditional<ShaderType == Current::shader_type,
                  typename make_type<Current, typename filter<ShaderType, Types...>::type>::type,
                  typename filter<ShaderType, Types...>::type
                  >::type;
          };

        public:
          shader_list() {}
          shader_list(const shader_list &o) : ct::type_list_member_instance<Shaders...>(o) {}
          shader_list(const cr::tuple<Shaders...> &o) : ct::type_list_member_instance<Shaders...>(o) {}

          template<typename... Vals>
          shader_list(Vals... vals) : ct::type_list_member_instance<Shaders...>(std::move(vals)...) {}


          // shaders by types
          using compute_shaders_t = typename filter<GL_COMPUTE_SHADER, Shaders...>::type;
          using fragment_shaders_t = typename filter<GL_FRAGMENT_SHADER, Shaders...>::type;
          using geometry_shaders_t = typename filter<GL_GEOMETRY_SHADER, Shaders...>::type;
          using tess_evaluation_shaders_t = typename filter<GL_TESS_EVALUATION_SHADER, Shaders...>::type;
          using tess_control_shaders_t = typename filter<GL_TESS_CONTROL_SHADER, Shaders...>::type;
          using vertex_shaders_t = typename filter<GL_VERTEX_SHADER, Shaders...>::type;

          // all shaders in one tuple
          using all_shaders_t = cr::tuple<Shaders...>;

          // the shader program
          template<typename... AdditionalShader>
          using program_t = neam::yaggler::shader::program<neam::yaggler::type::opengl, Shaders..., AdditionalShader...>;

          // shaders tuples by type
//           compute_shaders_t compute_shaders = compute_shaders_t();
//           vertex_shaders_t vertex_shaders = vertex_shaders_t();
//           tess_control_shaders_t tess_control_shaders = tess_control_shaders_t();
//           tess_evaluation_shaders_t tess_evaluation_shaders = tess_evaluation_shaders_t();
//           geometry_shaders_t geometry_shaders = geometry_shaders_t();
//           fragment_shaders_t fragment_shaders = fragment_shaders_t();
      };

    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_18450143281336995145_1567800396__CT_LIST_HPP__2___*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

