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
#include <png++/image.hpp>
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
            // pixel types
            // gray are converted to GL_RED, gray+alpha to GL_RG
            using __types__ = neam::ct::type_list<png::gray_pixel, png::gray_pixel, png::gray_pixel_16, png::ga_pixel, png::ga_pixel, png::ga_pixel_16,
                 png::rgb_pixel, png::rgb_pixel, png::rgb_pixel_16, png::rgba_pixel, png::rgba_pixel, png::rgba_pixel_16 >;
            static constexpr GLenum __values__ [] = {GL_R, GL_R8, GL_R16, GL_RG, GL_RG8, GL_RG16, GL_RGB, GL_RGB8, GL_RGB16, GL_RGBA, GL_RGBA8, GL_RGBA16};
            static constexpr size_t __default_value__ = 9; // index of png::rgba_pixel

            // gl format
            static constexpr GLenum __in_gl_format__ [] = {GL_R, GL_R, GL_R, GL_RG, GL_RG, GL_RG, GL_RGB, GL_RGB, GL_RGB, GL_RGBA, GL_RGBA, GL_RGBA};
            // gl type
            static constexpr GLenum __in_gl_type__ [] = {GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT};
            // comp num and size per pixel
            static constexpr GLenum __pixel_comp_num__ [] = {1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4};
            static constexpr GLenum __pixel_comp_size__ [] = {1, 1, 2};
            using __pixel_types__ = neam::ct::type_list < uint8_t, uint8_t, uint16_t >;

            // getter
            static constexpr size_t __rec_get_value_index(GLenum val, size_t idx, size_t max)
            {
              return __values__[idx] == val ? idx : (idx + 1 < max ? __rec_get_value_index(val, idx + 1, max) : __default_value__);
            }

            static constexpr size_t get_value_index(GLenum val)
            {
              return __rec_get_value_index(val, 0, sizeof(__values__) / sizeof(__values__[0]));
            }

            // types / values
            template<typename T>
            using remove_cref = typename std::remove_const<typename std::remove_reference<T>::type>::type;

            using png_image_type = remove_cref<typename __types__::get_type<get_value_index(InternalFormat)>>;
            using png_pixel_type = remove_cref<typename __pixel_types__::get_type<get_value_index(InternalFormat) % 3>>;

            static constexpr size_t comp_size = __pixel_comp_size__[get_value_index(InternalFormat) % (sizeof(__pixel_comp_size__) / sizeof(__pixel_comp_size__[0]))];
            static constexpr size_t comp_num = __pixel_comp_num__[get_value_index(InternalFormat) % (sizeof(__pixel_comp_num__) / sizeof(__pixel_comp_num__[0]))];
            static constexpr size_t pixel_size = comp_size * comp_num;

          private:
            png_texture_writer() = delete;
            png_texture_writer(png_texture_writer &) = delete;
            png_texture_writer & operator = (png_texture_writer &) = delete;

            // yes... I DO NOT EVEN copy the image pixel by pixel, but composant by composant... :'(
            template<typename T>
            inline static size_t copy_pixel(png::basic_ga_pixel<T> &pxl, png_pixel_type *array)
            {
              pxl.value = array[0];
              pxl.alpha = array[1];
              return 2;
            }
            template<typename T>
            inline static size_t copy_pixel(png::basic_rgb_pixel<T> &pxl, png_pixel_type *array)
            {
              pxl.red = array[0];
              pxl.green = array[1];
              pxl.blue = array[2];
              return 3;
            }
            template<typename T>
            inline static size_t copy_pixel(png::basic_rgba_pixel<T> &pxl, png_pixel_type *array)
            {
              pxl.red = array[0];
              pxl.green = array[1];
              pxl.blue = array[2];
              pxl.alpha = array[3];
              return 4;
            }
            // yep, a function to do a f*cking equal...
            inline static size_t copy_pixel(png_pixel_type &pxl, png_pixel_type *array)
            {
              pxl = array[0];
              return 1;
            }

            // write data to file...
            // worst. image writer. evar.
            static void set_data(const std::string &file, png_pixel_type *array, const neam::ct::vector2 &size)
            {
#ifndef YAGGLER_NO_MESSAGES
              neam::cr::chrono timer;
#endif
              png::image<png_image_type> image(size.x, size.y);

              image.set_filter_type(png::filter_type_base);
              image.set_compression_type(png::compression_type_base);

              size_t idx = 0;
              for (size_t y = 0; y < size.y; ++y)
              {
                for (size_t x = 0; x < size.x; ++x)
                {
                  png_image_type pixel;
                  idx += copy_pixel(pixel, array + idx);
                  image.set_pixel(x, size.y - (y + 1), pixel);
                }
              }

              image.write(file);

#ifndef YAGGLER_NO_MESSAGES
              std::cout << "YAGGLER:  saved png image '" << file << "' in " << timer.delta() << " seconds" << std::endl;
#endif
            }

          public:
            template<typename Texture>
            static bool write(const Texture &txt, const std::string &file)
            {
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

              uint8_t *data = new uint8_t[size.x * size.y * pixel_size];

              glGetTexImage(txt.get_texture_type(), Level, InternalFormat, GL_UNSIGNED_BYTE, data);

              try
              {
                set_data(file, reinterpret_cast<png_pixel_type *>(data), size);
              }
              catch(png::std_error &e)
              {
                std::cerr << "YAGGLER: saving png image '" << file << "': " << e.what() << std::endl;
                delete [] data;
                return false;
              }

              delete [] data;
              return true;
            }
        };
      } // namespace options
    } // namespace texture
  } // namespace yaggler
} // namespace neam

#endif /*__N_20466543071367365040_21856997__PNG_WRITER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

