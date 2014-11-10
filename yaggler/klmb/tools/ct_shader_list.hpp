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
#include <tools/merge_pack.hpp>
#include <tools/pair.hpp>

#include <shader/opengl_program.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      /// \brief a shader list. Sort shader by their types, and craft a shader program type
      /// the subclass \class program_auto_merger give the complete (and usable) program type
      /// from the \p Shaders template parameter and a 'main' shader list that'll be added if
      /// there is shaders of the same type in \p Shaders.
      template<typename... Shaders>
      struct shader_list : public ct::type_list<Shaders...>
      {
        private: // helpers
          // Thanks to : stackoverflow.com/q/18366398/
          template<GLenum ShaderType, typename...>
          struct filter
          {
            using type = cr::tuple<>;
          };
          template<GLenum ShaderType, typename Current, typename... Types>
          struct filter<ShaderType, Current, Types...>
          {
            using type = typename std::conditional<ShaderType == Current::shader_type,
                  typename ct::append_type<Current, typename filter<ShaderType, Types...>::type>::type,
                  typename filter<ShaderType, Types...>::type
                  >::type;
          };

        public:
          constexpr shader_list() {}
          constexpr shader_list(const shader_list &o) : ct::type_list_member_instance<Shaders...>(o) {}
          constexpr shader_list(const cr::tuple<Shaders...> &o) : ct::type_list_member_instance<Shaders...>(o) {}

          template<typename... Vals>
          constexpr shader_list(Vals... vals) : ct::type_list_member_instance<Shaders...>(std::move(vals)...) {}


          // shaders by types
          using fragment_shaders_t = typename filter<GL_FRAGMENT_SHADER, Shaders...>::type;
          using geometry_shaders_t = typename filter<GL_GEOMETRY_SHADER, Shaders...>::type;
          using tess_evaluation_shaders_t = typename filter<GL_TESS_EVALUATION_SHADER, Shaders...>::type;
          using tess_control_shaders_t = typename filter<GL_TESS_CONTROL_SHADER, Shaders...>::type;
          using vertex_shaders_t = typename filter<GL_VERTEX_SHADER, Shaders...>::type;
          using compute_shaders_t = typename filter<GL_COMPUTE_SHADER, Shaders...>::type;

          // all shaders in one tuple
          using all_shaders_t = cr::tuple<Shaders...>;

          // the shader program
          template<typename... AdditionalShaders>
          using program_t = neam::yaggler::shader::program<neam::yaggler::type::opengl, Shaders..., AdditionalShaders...>;

          /// \brief merge each shaders of \p Values only if \p Shaders contain a shader of the same type.
          /// \param Values a serie of \code ct::pair<X, Y> \endcode [with X the shader type (embed) and Y the \code shader::shader<opengl> \endcode to append]
          template<typename... Values>
          struct program_auto_merger
          {
            private:
              template<typename...>
              struct am_filter
              {
                using type = cr::tuple<>;
              };
              template<typename Current, typename... Types>
              struct am_filter<Current, Types...>
              {
                using type = typename std::conditional < (bool)filter<Current::type_1::value, Shaders...>::type::size(),
                      typename ct::append_type<typename Current::type_2, typename am_filter<Types...>::type>::type,
                      typename am_filter<Types...>::type
                      >::type;
              };

            public:
              using type = typename ct::extract_types<program_t, typename am_filter<Values...>::type>::type;
          };
      };

    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_18450143281336995145_1567800396__CT_LIST_HPP__2___*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

