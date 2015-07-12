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
      /// \brief used to check cubemap faces at compile time
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

        /// \brief init from a constexpr source (from a  static constexpr array of pixel)
        /// \param InternalFormat The internal format of the openGL texture
        /// \param Vector The size of the texture (type: ct::vector<>, 1, 2 or 3 dimensions)
        /// \param Type The texture size (unsigned byte or unsigned short)
        /// \param Data The texture data
        /// \param Level The mipmap level holding in the png file
        template<GLint InternalFormat, typename Vector, GLenum Format, GLenum Type, typename Data, size_t Level = 0>
        class ct_texture_init
        {
          private:
          public:
            constexpr ct_texture_init() {}

            static constexpr typename Vector::vec_type size = Vector::value;
            static constexpr GLint internal_format = InternalFormat; ///< \brief The texture internal format (as in the InternalFormat template parameter )
            static constexpr size_t level = Level; ///< \brief The mipmap level (as in the Level template parameter)
            static constexpr GLenum format = Format; ///< \brief The format of the texure (R, RG, RGB, RGBA)
            static constexpr GLenum type = Type; ///< \brief The texture size (unsigned byte or unsigned short)
            static constexpr GLvoid *data = (GLvoid *)(Data::value); ///< \brief The actual texture data

            static constexpr bool is_init = true; ///< \brief Always true as the data object is a static property
        };

        template<GLint InternalFormat, typename Vector, GLenum Format, GLenum Type, typename Data, size_t Level>
        constexpr GLvoid *ct_texture_init<InternalFormat, Vector, Format, Type, Data, Level>::data;

        template<GLint InternalFormat, typename Vector, GLenum Format, GLenum Type, typename Data, size_t Level>
        constexpr typename Vector::vec_type ct_texture_init<InternalFormat, Vector, Format, Type, Data, Level>::size;

        /// \brief Init an empty texture
        /// \param InternalFormat The internal format of the openGL texture
        /// \param Vector The size of the texture (type: ct::vector<>, 1, 2 or 3 dimensions)
        /// \param Level The mipmap level holding in the png file
        template<GLint InternalFormat, typename Vector, size_t Level = 0>
        class empty_texture_init
        {
          private:
          public:
            constexpr empty_texture_init() {}

            static constexpr typename Vector::vec_type size = Vector::value;
            static constexpr GLint internal_format = InternalFormat; ///< \brief The texture internal format (as in the InternalFormat template parameter )
            static constexpr size_t level = Level; ///< \brief The mipmap level (as in the Level template parameter)
            static constexpr GLenum format = InternalFormat == GL_DEPTH_COMPONENT || ///< \brief The format of the texure (R, RG, RGB, RGBA)
                                             InternalFormat == GL_DEPTH_COMPONENT16 ||
                                             InternalFormat == GL_DEPTH_COMPONENT24 || InternalFormat == GL_DEPTH_COMPONENT32F ? GL_DEPTH_COMPONENT : GL_RED;
            static constexpr GLenum type = GL_BYTE; ///< \brief The texture size (unsigned byte or unsigned short)
            static constexpr GLvoid *data = nullptr; ///< \brief The actual texture data (none in this case)

            static constexpr bool is_init = true; ///< \brief Always true as the data property is a static property
        };

        template<GLint InternalFormat, typename Vector, size_t Level>
        constexpr GLvoid *empty_texture_init<InternalFormat, Vector, Level>::data;

        template<GLint InternalFormat, typename Vector, size_t Level>
        constexpr typename Vector::vec_type empty_texture_init<InternalFormat, Vector, Level>::size;

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

