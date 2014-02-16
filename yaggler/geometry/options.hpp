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


#ifndef __N_2581455562078652210_192587929__OPTIONS_HPP__
# define __N_2581455562078652210_192587929__OPTIONS_HPP__

#include <GLEW/glew.h>
#include <ct_point.hpp>

#include <tools/embed.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace geometry
    {
      // used in buffer_view
      enum class destination_precision
      {
        integer,
        single_precision,
        double_precision
      };

#define NYG_UNUSABLE_OPTION_CLASS(c)  class c {c() = delete; ~c() = delete;};
      // some cool options for ct/- init
      namespace options
      {
        // ct view init
        template<GLuint AttributeIndex, GLuint Size, GLenum Type, GLuint Stride, GLuint Offset, bool Normalized = false>
        struct ct_buffer_view_init
        {
          static constexpr GLuint attribute_index = AttributeIndex;
          static constexpr GLuint size = Size;
          static constexpr GLenum type = Type;
          static constexpr GLuint stride = Stride;
          static constexpr GLuint offset = Offset;
          static constexpr bool normalized = Normalized;
        };

        // ct buffer init
        template<typename Data, GLenum DrawType, bool Convert = false>
        struct ct_buffer_init
        {
          private:
            using data_t = typename std::remove_const<decltype(Data::value)>::type;

          public:
            static constexpr data_t data = Data::value;
            static constexpr GLenum draw_type = DrawType;
            static constexpr bool convert = Convert;
        };

        template<typename Data, GLenum DrawType, bool Convert>
        constexpr typename ct_buffer_init<Data, DrawType, Convert>::data_t ct_buffer_init<Data, DrawType, Convert>::data;

        // ct vao init
        // (and here, you can see the powa of those ct inits :) )
        template<typename Buffer, typename... Views>
        struct ct_vao_init
        {
          public:
            using buffer_type = Buffer;

            Buffer buffer = Buffer();
            static constexpr cr::tuple<Views...> views = cr::tuple<Views...>(Views()...);

            void init() const
            {
              buffer.use();
              _use_views();
            }

            static void _use_views()
            {
              _it_use_views(cr::gen_seq<sizeof...(Views)>());
            }

          private:
            template<size_t Idx>
            static int8_t _it_use_view_single()
            {
              views.template get<Idx>().use();
              return 0;
            }

            template<size_t... Idxs>
            static void _it_use_views(neam::cr::seq<Idxs...>)
            {
              void((char []){_it_use_view_single<Idxs>()...}); // who knows how this'll be optimised out ?
              // (and which compiler supports it...)
            }
        };

        template<typename Buffer, typename... Views>
        constexpr cr::tuple<Views...>  ct_vao_init<Buffer, Views...>::views;

      } // namespace options
#undef NYG_UNUSABLE_OPTION_CLASS
    } // namespace texture

  } // namespace geometry
  namespace embed
  {
    namespace geometry
    {
      N_EMBED_USING(destination_precision, yaggler::geometry::destination_precision);
    } // namespace geometry
  } // namespace embed
} // namespace neam

#endif /*__N_2581455562078652210_192587929__OPTIONS_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

