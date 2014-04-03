//
// file : png_loader.hpp
// in : file:///home/tim/projects/yaggler/yaggler/texture/png_loader.hpp
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


#ifndef __N_748986457882865260_592567929__PNG_LOADER_HPP__
# define __N_748986457882865260_592567929__PNG_LOADER_HPP__

#include <GLEW/glew.h>
#include <lodePNG/lodepng.h>
#include <ct_point.hpp>

#include <tools/embed.hpp>
#include <tools/tuple.hpp>
#include <tools/chrono.hpp>
#include "ct_fixed.hpp"
#include "tools/ct_list.hpp"
#include <yaggler_except.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace texture
    {
      // some cool options for png init
      namespace options
      {
        /// ### texture options

        // tell the texture to init, and how to init. (could be placed multiple times to init each levels and/or cubemap faces)
        // see texture<opengl>::set_texture_data for explanation of the tpl params

        // init from a png file source
        template<GLenum InternalFormat, typename File, size_t Level = 0>
        class png_texture_init
        {
          private:
            static constexpr GLenum gl_color_type[] = {GL_R8, GL_R16, GL_RG8, GL_RG16, GL_RGB8, GL_RGB16, GL_RGBA8, GL_RGBA16};
            static constexpr LodePNGColorType png_color_type[] = {LCT_GREY, LCT_GREY_ALPHA, LCT_RGB, LCT_RGBA};
            static constexpr GLenum gl_input_color_type[] = {GL_R, GL_RG, GL_RGB, GL_RGBA};
            static constexpr unsigned int png_depth[] = {8, 16};
            static constexpr GLenum gl_depth[] = {GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT};

            static constexpr size_t _get_index(size_t cr_index = 0)
            {
              return gl_color_type[cr_index] == InternalFormat ? cr_index : (cr_index + 1 < sizeof(gl_color_type) / sizeof(*gl_color_type) ? _get_index(cr_index + 1) : sizeof(gl_color_type) / sizeof(*gl_color_type) - 1);
            }

          private:
            static void *get_data(neam::ct::vector2 &size)
            {
#ifndef YAGGLER_NO_MESSAGES
              neam::cr::chrono timer;
#endif

              std::vector<unsigned char> image;
              unsigned int szx = size.x;
              unsigned int szy = size.y;
              unsigned int ret = lodepng::decode(image, szx, szy, File::value, static_cast<LodePNGColorType>(png_color_type[_get_index() / 2]), png_depth[_get_index() % 2]);
              size.x = szx;
              size.y = szy;

              if (ret)
                throw runtime_error<png_texture_init>(lodepng_error_text(ret));

              uint8_t *data = new uint8_t[size.x * size.y * 8];
              memcpy(data, image.data(), size.x * size.y * 8);

#ifndef YAGGLER_NO_MESSAGES
              std::cout << "YAGGLER: loaded png image '" << File::value << "' in " << timer.delta() << " seconds" << std::endl;
#endif
              return data;
            }

          public:
            png_texture_init()
              : size(), data(get_data(size))
            {
            }

            ~png_texture_init()
            {
              delete [] reinterpret_cast<uint8_t *>(data);
            }

            neam::ct::vector2 size;
            static constexpr GLint internal_format = InternalFormat;
            static constexpr size_t level = Level;

            // automatically generated from the InternalFormat field and the "best" value for it in png++.
            static constexpr GLenum format = gl_input_color_type[_get_index() / 2];
            static constexpr GLenum type = gl_depth[_get_index() % 2];

            GLvoid *data;

            static constexpr bool is_init = true;
        };
        template<GLenum InternalFormat, typename File, size_t Level>
        constexpr GLenum png_texture_init<InternalFormat, File, Level>::gl_color_type[];
        template<GLenum InternalFormat, typename File, size_t Level>
        constexpr GLenum png_texture_init<InternalFormat, File, Level>::gl_input_color_type[];
        template<GLenum InternalFormat, typename File, size_t Level>
        constexpr unsigned int png_texture_init<InternalFormat, File, Level>::png_depth[];
        template<GLenum InternalFormat, typename File, size_t Level>
        constexpr LodePNGColorType png_texture_init<InternalFormat, File, Level>::png_color_type[];
        template<GLenum InternalFormat, typename File, size_t Level>
        constexpr GLenum png_texture_init<InternalFormat, File, Level>::gl_depth[];


        template<GLenum InternalFormat, typename File, size_t Level>
        constexpr GLint png_texture_init<InternalFormat, File, Level>::internal_format;
        template<GLenum InternalFormat, typename File, size_t Level>
        constexpr size_t png_texture_init<InternalFormat, File, Level>::level;
        template<GLenum InternalFormat, typename File, size_t Level>
        constexpr GLenum png_texture_init<InternalFormat, File, Level>::format;
        template<GLenum InternalFormat, typename File, size_t Level>
        constexpr GLenum png_texture_init<InternalFormat, File, Level>::type;
        template<GLenum InternalFormat, typename File, size_t Level>
        constexpr bool png_texture_init<InternalFormat, File, Level>::is_init;

        // load from a png file source
        template<GLenum InternalFormat, size_t Level = 0>
        class png_texture_loader
        {
          private:
            static constexpr GLenum gl_color_type[] = {GL_R8, GL_R16, GL_RG8, GL_RG16, GL_RGB8, GL_RGB16, GL_RGBA8, GL_RGBA16};
            static constexpr LodePNGColorType png_color_type[] = {LCT_GREY, LCT_GREY_ALPHA, LCT_RGB, LCT_RGBA};
            static constexpr GLenum gl_input_color_type[] = {GL_R, GL_RG, GL_RGB, GL_RGBA};
            static constexpr unsigned int png_depth[] = {8, 16};
            static constexpr GLenum gl_depth[] = {GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT};

            static constexpr size_t _get_index(size_t cr_index = 0)
            {
              return gl_color_type[cr_index] == InternalFormat ? cr_index : (cr_index + 1 < sizeof(gl_color_type) / sizeof(*gl_color_type) ? _get_index(cr_index + 1) : sizeof(gl_color_type) / sizeof(*gl_color_type) - 1);
            }

          private:
            static void *get_data(neam::ct::vector2 &size, const std::string &file)
            {
#ifndef YAGGLER_NO_MESSAGES
              neam::cr::chrono timer;
#endif

              std::vector<unsigned char> image;
              unsigned int szx = size.x;
              unsigned int szy = size.y;
              unsigned int ret = lodepng::decode(image, szx, szy, file, static_cast<LodePNGColorType>(png_color_type[_get_index() / 2]), png_depth[_get_index() % 2]);
              size.x = szx;
              size.y = szy;

              if (ret)
                throw runtime_error<png_texture_loader>(lodepng_error_text(ret));

              uint8_t *data = new uint8_t[size.x * size.y * 8];
              memcpy(data, image.data(), image.size());

#ifndef YAGGLER_NO_MESSAGES
              std::cout << "YAGGLER: loaded png image '" << file << "' in " << timer.delta() << " seconds" << std::endl;
#endif
              return data;
            }

          public:
            png_texture_loader(const std::string &file)
              : size(), data(get_data(size, file))
            {
            }

            ~png_texture_loader()
            {
              delete [] reinterpret_cast<uint8_t *>(data);
            }

            neam::ct::vector2 size;
            static constexpr GLint internal_format = InternalFormat;
            static constexpr size_t level = Level;

            // automatically generated from the InternalFormat field and the "best" value for it in png++.
            static constexpr GLenum format = gl_input_color_type[_get_index() / 2];
            static constexpr GLenum type = gl_depth[_get_index() % 2];

            GLvoid *data;

            static constexpr bool is_init = true;
        };
        template<GLenum InternalFormat, size_t Level>
        constexpr GLenum png_texture_loader<InternalFormat, Level>::gl_color_type[];
        template<GLenum InternalFormat, size_t Level>
        constexpr GLenum png_texture_loader<InternalFormat, Level>::gl_input_color_type[];
        template<GLenum InternalFormat, size_t Level>
        constexpr unsigned int png_texture_loader<InternalFormat, Level>::png_depth[];
        template<GLenum InternalFormat, size_t Level>
        constexpr LodePNGColorType png_texture_loader<InternalFormat, Level>::png_color_type[];
        template<GLenum InternalFormat, size_t Level>
        constexpr GLenum png_texture_loader<InternalFormat, Level>::gl_depth[];


        template<GLenum InternalFormat, size_t Level>
        constexpr GLint png_texture_loader<InternalFormat, Level>::internal_format;
        template<GLenum InternalFormat, size_t Level>
        constexpr size_t png_texture_loader<InternalFormat, Level>::level;
        template<GLenum InternalFormat, size_t Level>
        constexpr GLenum png_texture_loader<InternalFormat, Level>::format;
        template<GLenum InternalFormat, size_t Level>
        constexpr GLenum png_texture_loader<InternalFormat, Level>::type;
        template<GLenum InternalFormat, size_t Level>
        constexpr bool png_texture_loader<InternalFormat, Level>::is_init;

      } // namespace options
    } // namespace texture

  } // namespace yaggler
  // for embeding cubemap_face in template params (required when ct initialising cubemaps)
  namespace embed
  {
    namespace texture
    {
    } // namespace texture
  } // namespace embed
} // namespace neam

#endif /*__N_748986457882865260_592567929__PNG_LOADER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

