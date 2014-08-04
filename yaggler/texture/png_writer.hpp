//
// file : png_writer.hpp
// in : file:///home/tim/projects/yaggler/yaggler/texture/png_writer.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 19/03/2014 16:31:17
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

#ifndef __N_20466543071367365040_21856997__PNG_WRITER_HPP__
# define __N_20466543071367365040_21856997__PNG_WRITER_HPP__

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
      namespace options
      {
        template<GLint InternalFormat, size_t Level = 0>
        class png_texture_writer
        {
          private:
            static constexpr GLenum gl_color_type[] = {GL_R8, GL_R16, GL_RG8, GL_RG16, GL_RGB8, GL_RGB16, GL_RGBA8, GL_RGBA16};
            static constexpr size_t pixel_sz[] = {1, 2, 2, 4, 3, 6, 4, 8};
            static constexpr LodePNGColorType png_color_type[] = {LCT_GREY, LCT_GREY_ALPHA, LCT_RGB, LCT_RGBA};
            static constexpr GLenum gl_input_color_type[] = {GL_R, GL_RG, GL_RGB, GL_RGBA};
            static constexpr int png_depth[] = {8, 16};
            static constexpr GLenum gl_depth[] = {GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT};

            static constexpr size_t _get_index(size_t cr_index = 0)
            {
              return gl_color_type[cr_index] == InternalFormat ? cr_index : (cr_index + 1 < sizeof(gl_color_type) / sizeof(*gl_color_type) ? _get_index(cr_index + 1) : sizeof(gl_color_type) / sizeof(*gl_color_type) - 1);
            }

          public:
            template<typename Texture>
            static bool write(const Texture &txt, const std::string &file, bool invert_y = false)
            {
#ifndef YAGGLER_NO_MESSAGES
              neam::cr::chrono timer;
#endif
              // get width and height of the texture
              neam::ct::vector2 size;
              GLint tmp = 0;

              txt.bind();
              glGetTexLevelParameteriv(txt.get_texture_type(), Level, GL_TEXTURE_WIDTH, &tmp);
              size.x = tmp;
              glGetTexLevelParameteriv(txt.get_texture_type(), Level, GL_TEXTURE_HEIGHT, &tmp);
              size.y = tmp;

              if (!size.x || !size.y)
                return false;

              uint8_t *data = new uint8_t[size.x * size.y * pixel_sz[_get_index()]];

              glGetTexImage(txt.get_texture_type(), Level, gl_input_color_type[_get_index() / 2], gl_depth[_get_index() % 2], data);

              // swap y pixels
              if (invert_y)
              {
                for (neam::ct::vector2 it(0, 0); it.y < size.y / 2; ++it.y)
                {
                  for (it.x = 0; it.x < size.x; ++it.x)
                  {
                    for (size_t i = 0; i < pixel_sz[_get_index()]; ++i)
                      std::swap((data)[(it.x + size.x * it.y) * pixel_sz[_get_index()] + i], (data)[(it.x + size.x * (size.y - it.y - 1)) * pixel_sz[_get_index()] + i]);
                  }
                }
              }

              unsigned int ret = lodepng_encode_file(file.data(), data, size.x, size.y, png_color_type[_get_index() / 2], png_depth[_get_index() % 2]);

              delete [] data;

              if (ret)
                throw runtime_error<png_texture_writer>(lodepng_error_text(ret));

#ifndef YAGGLER_NO_MESSAGES
              std::cout << "YAGGLER: saved png image  '" << file << "' in " << timer.delta() << " seconds" << std::endl;
#endif

              return true;
            }
        };
        template<GLint InternalFormat, size_t Level> constexpr GLenum png_texture_writer<InternalFormat, Level>::gl_color_type[];
        template<GLint InternalFormat, size_t Level> constexpr size_t png_texture_writer<InternalFormat, Level>::pixel_sz[];
        template<GLint InternalFormat, size_t Level> constexpr LodePNGColorType png_texture_writer<InternalFormat, Level>::png_color_type[];
        template<GLint InternalFormat, size_t Level> constexpr GLenum png_texture_writer<InternalFormat, Level>::gl_input_color_type[];
        template<GLint InternalFormat, size_t Level> constexpr int png_texture_writer<InternalFormat, Level>::png_depth[];
        template<GLint InternalFormat, size_t Level> constexpr GLenum png_texture_writer<InternalFormat, Level>::gl_depth[];

      } // namespace options
    } // namespace texture
  } // namespace yaggler
} // namespace neam

#endif /*__N_20466543071367365040_21856997__PNG_WRITER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

