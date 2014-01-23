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
#include <png++/image.hpp>
#include <ct_point.hpp>

#include <tools/embed.hpp>
#include <tools/tuple.hpp>
#include <tools/chrono.hpp>
#include "../ct_fixed.hpp"

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
        // see texture<opengl>::set_texture_data for explanation of params

        // init from a png file source
        //
        // this isn't a good piece of code... (due to the underlying lib used here)
        // but it works.
        template<GLint InternalFormat, typename File, size_t Level = 0>
        class png_texture_init
        {
          private:
            // pixel types
            // gray are converted to GL_RED, gray+alpha to GL_RG
            neam::cr::tuple<png::gray_pixel, png::gray_pixel, png::gray_pixel_16, png::ga_pixel, png::ga_pixel, png::ga_pixel_16,
                 png::rgb_pixel, png::rgb_pixel, png::rgb_pixel_16, png::rgba_pixel, png::rgba_pixel, png::rgba_pixel_16 > __types__;
            static constexpr GLenum __values__ [] = {GL_R, GL_R8, GL_R16, GL_RG, GL_RG8, GL_RG16, GL_RGB, GL_RGB8, GL_RGB16, GL_RGBA, GL_RGBA8, GL_RGBA16};
            static constexpr size_t __default_value__ = 9; // index of png::rgba_pixel

            // gl format
            static constexpr GLenum __in_gl_format__ [] = {GL_R, GL_R, GL_R, GL_RG, GL_RG, GL_RG, GL_RGB, GL_RGB, GL_RGB, GL_RGBA, GL_RGBA, GL_RGBA};
            // gl type
            static constexpr GLenum __in_gl_type__ [] = {GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT};
            // comp num and size per pixel
            static constexpr GLenum __pixel_comp_num__ [] = {1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4};
            static constexpr GLenum __pixel_comp_size__ [] = {1, 1, 2};
            neam::cr::tuple < uint8_t, uint8_t, uint16_t > __pixel_types__;

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

            using png_image_type = remove_cref<decltype(__types__.get<get_value_index(InternalFormat)>())>;
            using png_pixel_type = remove_cref<decltype(__pixel_types__.get<get_value_index(InternalFormat) % 3>())>;

            static constexpr size_t comp_size = __pixel_comp_size__[get_value_index(InternalFormat) % (sizeof(__pixel_comp_size__) / sizeof(__pixel_comp_size__[0]))];
            static constexpr size_t comp_num = __pixel_comp_num__[get_value_index(InternalFormat) % (sizeof(__pixel_comp_num__) / sizeof(__pixel_comp_num__[0]))];
            static constexpr size_t pixel_size = comp_size * comp_num;

          private:
            png_texture_init(png_texture_init &) = delete;
            png_texture_init & operator = (png_texture_init &) = delete;

            // yes... I DO NOT EVEN copy the image pixel by pixel, but composant by composant... :'(
            template<typename T>
            inline static size_t copy_pixel(const png::basic_ga_pixel<T> &pxl, png_pixel_type *array)
            {
              array[0] = pxl.value;
              array[1] = pxl.alpha;
              return 2;
            }
            template<typename T>
            inline static size_t copy_pixel(const png::basic_rgb_pixel<T> &pxl, png_pixel_type *array)
            {
              array[0] = pxl.red;
              array[1] = pxl.green;
              array[2] = pxl.blue;
              return 3;
            }
            template<typename T>
            inline static size_t copy_pixel(const png::basic_rgba_pixel<T> &pxl, png_pixel_type *array)
            {
              array[0] = pxl.red;
              array[1] = pxl.green;
              array[2] = pxl.blue;
              array[3] = pxl.alpha;
              return 4;
            }
            // yep, a function to do a f*cking equal...
            inline static size_t copy_pixel(png_pixel_type pxl, png_pixel_type *array)
            {
              array[0] = pxl;
              return 1;
            }

            // please, call delete [] on the returned array...
            static GLvoid *get_data(neam::ct::vector2 &size)
            {
#ifndef YAGGLER_NO_MESSAGES
              neam::cr::chrono timer;
#endif

              png::image<png_image_type> image(File::value);
              size.x = image.get_width();
              size.y = image.get_height();

// #ifndef YAGGLER_NO_MESSAGES
//               std::cout << "YAGGLER: loading png image '" << File::value << "' [" << size.x << ", " << size.y << "] pixels (with " << pixel_size * 8 << "bpp)..." << std::endl;
// #endif

              uint8_t *ret_array = new uint8_t[pixel_size * size.x * size.y];

              size_t idx = 0;

              for (ct::fixed_t y = 0; y < size.y; ++y)
              {
                for (ct::fixed_t x = 0; x < size.x; ++x)
                {
                  idx += comp_size * copy_pixel(image[size.y - (y + 1)][x], reinterpret_cast<png_pixel_type *>(ret_array + idx));
                }
              }

#ifndef YAGGLER_NO_MESSAGES
              std::cout << "YAGGLER: loaded png image '" << File::value << "' in " << timer.delta() << " seconds" << std::endl;
#endif
              return ret_array;
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
            static constexpr GLenum format = __in_gl_format__[get_value_index(InternalFormat)];
            static constexpr GLenum type = __in_gl_type__[get_value_index(InternalFormat) % (sizeof(__in_gl_type__) / sizeof(__in_gl_type__[0]))];

            GLvoid *data;

            static constexpr bool is_init = true;
        };


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

