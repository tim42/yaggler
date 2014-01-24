//
// file : options.hpp
// in : file:///home/tim/projects/yaggler/yaggler/texture/options.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 15/01/2014 14:14:46
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


#ifndef __N_789864578682865260_592567929__OPTIONS_HPP__
# define __N_789864578682865260_592567929__OPTIONS_HPP__

#include <GLEW/glew.h>
#include <ct_point.hpp>

#include <tools/embed.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace texture
    {
      // used to check cubemap faces at compile time
      enum class cubemap_face : GLenum
      {
        positive_x = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        positive_y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        positive_z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        negative_x = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        negative_y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        negative_z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
      };

#define NYT_UNUSABLE_OPTION_CLASS(c)  class c {c() = delete; ~c() = delete;};
      // some cool options for ct init
      namespace options
      {
        /// ### texture options

        // tell the texture to init, and how to init. (could be placed multiple times to init each levels and/or cubemap faces)
        // see texture<opengl>::set_texture_data for explanation of params

        // init from a ct source (--> static constexpr array of pixel)
        template<GLint InternalFormat, typename Vector, GLenum Format, GLenum Type, typename Data, size_t Level = 0>
        class ct_texture_init
        {
          private:
          public:
            constexpr ct_texture_init() {}

            static constexpr typename Vector::vec_type size = Vector::value;
            static constexpr GLint internal_format = InternalFormat;
            static constexpr size_t level = Level;
            static constexpr GLenum format = Format;
            static constexpr GLenum type = Type;
            static constexpr GLvoid *data = (GLvoid *)(Data::value);

            static constexpr bool is_init = true;
        };

        template<GLint InternalFormat, typename Vector, GLenum Format, GLenum Type, typename Data, size_t Level>
        constexpr GLvoid *ct_texture_init<InternalFormat, Vector, Format, Type, Data, Level>::data;

        template<GLint InternalFormat, typename Vector, GLenum Format, GLenum Type, typename Data, size_t Level>
        constexpr typename Vector::vec_type ct_texture_init<InternalFormat, Vector, Format, Type, Data, Level>::size;

        // init an empty texture
        template<GLint InternalFormat, typename Vector, size_t Level = 0>
        class empty_texture_init
        {
          private:
          public:
            constexpr empty_texture_init() {}

            static constexpr typename Vector::vec_type size = Vector::value;
            static constexpr GLint internal_format = InternalFormat;
            static constexpr size_t level = Level;
            static constexpr GLenum format = InternalFormat == GL_DEPTH_COMPONENT || InternalFormat == GL_DEPTH_COMPONENT16 ||
                                             InternalFormat == GL_DEPTH_COMPONENT24 || InternalFormat == GL_DEPTH_COMPONENT32F ? GL_DEPTH_COMPONENT : GL_RED;
            static constexpr GLenum type = GL_BYTE;
            static constexpr GLvoid *data = nullptr;

            static constexpr bool is_init = true;
        };

        template<GLint InternalFormat, typename Vector, size_t Level>
        constexpr GLvoid *empty_texture_init<InternalFormat, Vector, Level>::data;

        template<GLint InternalFormat, typename Vector, size_t Level>
        constexpr typename Vector::vec_type empty_texture_init<InternalFormat, Vector, Level>::size;

        /// ### frame buffer options

        

      } // namespace options
#undef NYT_UNUSABLE_OPTION_CLASS
    } // namespace texture

  } // namespace yaggler
  // for embeding cubemap_face in template params (required when ct initialising cubemaps)
  namespace embed
  {
    namespace texture
    {
      N_EMBED_USING(cubemap_face, yaggler::texture::cubemap_face);
    } // namespace texture
  } // namespace embed
} // namespace neam

#endif /*__N_789864578682865260_592567929__OPTIONS_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

