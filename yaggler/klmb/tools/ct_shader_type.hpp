//
// file : ct_shader_type.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/tools/ct_shader_type.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 06/02/2014 09:30:25
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

#ifndef __N_3400797801128414818_272837570__CT_SHADER_TYPE_HPP__
# define __N_3400797801128414818_272837570__CT_SHADER_TYPE_HPP__

#include <GLEW/glew.h>
#include <tools/ct_string.hpp>
#include <tools/genseq.hpp>


// default YägGLer shader extensions are:
//
// .frag        --> fragment shader
// .geom        --> geometry shader
// .tes         --> tesselation evaluation shader
// .tcs         --> tesselation control shader
// .vert        --> vertex shader
//
// .compute     --> compute shader
//

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      namespace internal
      {
        constexpr size_t _ct_str_get_dot_index(const char *str, size_t rmg)
        {
          return str[rmg] == '.' ? rmg : (rmg ? _ct_str_get_dot_index(str, rmg - 1) : 0);
        }

        /// \brief return the index of the last dot of a filename
        constexpr size_t ct_str_get_dot_index(const char *str)
        {
          return _ct_str_get_dot_index(str, ct::strlen(str));
        }

        template<char... Letters>
        struct shader_type_by_ext
        {
          private:
            // this isn't a valid extension !!!
            static constexpr GLenum shader_type = 0;
        };

        /// \brief fragment shader
        template<>
        struct shader_type_by_ext<'.', 'f', 'r', 'a', 'g'>
        {
          static constexpr GLenum shader_type = GL_FRAGMENT_SHADER;
        };

        /// \brief geometry shader
        template<>
        struct shader_type_by_ext<'.', 'g', 'e', 'o', 'm'>
        {
          static constexpr GLenum shader_type = GL_GEOMETRY_SHADER;
        };

        /// \brief tess evaluation shader
        template<>
        struct shader_type_by_ext<'.', 't', 'e', 's'>
        {
          static constexpr GLenum shader_type = GL_TESS_EVALUATION_SHADER;
        };

        /// \brief tess control shader
        template<>
        struct shader_type_by_ext<'.', 't', 'c', 's'>
        {
          static constexpr GLenum shader_type = GL_TESS_CONTROL_SHADER;
        };

        /// \brief vertex shader
        template<>
        struct shader_type_by_ext<'.', 'v', 'e', 'r', 't'>
        {
          static constexpr GLenum shader_type = GL_VERTEX_SHADER;
        };

        /// \brief compute shader
        template<>
        struct shader_type_by_ext<'.', 'c', 'o', 'm', 'p', 'u', 't', 'e'>
        {
          static constexpr GLenum shader_type = GL_COMPUTE_SHADER;
        };

        /// \brief deduce the shader type from its filename
        template<const char *Str>
        struct shader_type_from_filename
        {
          private:
            template<size_t DotIdx, size_t... Idxs>
            static constexpr GLenum get_shader_type(cr::seq<Idxs...>)
            {
              return shader_type_by_ext<Str[DotIdx + Idxs]...>::shader_type;
            }

          public:
            static constexpr GLenum shader_type = get_shader_type<ct_str_get_dot_index(Str)>(cr::gen_seq<ct::strlen(Str + ct_str_get_dot_index(Str))>());
        };
      } // namespace internal
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_3400797801128414818_272837570__CT_SHADER_TYPE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

