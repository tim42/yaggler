//
// file : opengl_draw_state.hpp
// in : file:///home/tim/projects/yaggler/yaggler/geometry/opengl_draw_state.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 15/02/2014 10:55:00
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

#ifndef __N_18391655302072008314_1562393302__OPENGL_DRAW_STATE_HPP__
# define __N_18391655302072008314_1562393302__OPENGL_DRAW_STATE_HPP__

#include <yaggler_type.hpp>
#include <geometry/draw_state_base.hpp>
#include <GLEW/glew.h>

namespace neam
{
  namespace yaggler
  {
    namespace geometry
    {
      enum class draw_method
      {
        normal,
        indexed,
        instanced,
        indexed_instanced
      };

      template<>
      class draw_state<type::opengl>
      {
        public:
          draw_state(draw_method _method = draw_method::normal)
            : method(_method), mode(0), count(0), type(GL_UNSIGNED_INT), start_index(0), prim_count(1)
          {
          }

          void set_draw_triangles(size_t triangle_count)
          {
            mode = GL_TRIANGLES;
            count = triangle_count * 3;
          }
          void set_draw_lines(size_t line_count)
          {
            mode = GL_LINE;
            count = line_count * 2;
          }
          void set_draw_points(size_t point_count)
          {
            mode = GL_POINTS;
            count = point_count;
          }

          void set_draw(GLenum _mode, size_t _count)
          {
            mode = _mode;
            count = _count;
          }

          void set_draw_method(draw_method _method)
          {
            method = _method;
          }

          draw_method get_draw_method() const
          {
            return method;
          }

          void set_index_type(GLenum _type)
          {
            type = _type;
          }

          void set_start_index(size_t _index)
          {
            start_index = _index;
          }

          void set_primitive_count(size_t _prim_count)
          {
            prim_count = _prim_count;
          }

          GLenum get_mode() const
          {
            return mode;
          }

          size_t get_count() const
          {
            return count;
          }

          GLenum get_index_type() const
          {
            return type;
          }

          size_t get_start_index() const
          {
            return start_index;
          }

          size_t get_primitive_count() const
          {
            return prim_count;
          }

          void draw() const
          {
            switch (method)
            {
              case draw_method::normal:
                glDrawArrays(mode, start_index, type);
                break;
              case draw_method::indexed:
                glDrawElements(mode, count, type, reinterpret_cast<GLvoid *>(start_index));
                break;
              case draw_method::instanced:
                glDrawArraysInstanced(mode, start_index, type, prim_count);
                break;
              case draw_method::indexed_instanced:
                glDrawElementsInstanced(mode, count, type, reinterpret_cast<GLvoid *>(start_index), prim_count);
                break;
#ifndef YAGGLER_NO_MESSAGES
              default:
                neam::cr::out.error() << LOGGER_INFO << "unknow draw method" << std::endl;
#endif
            }
          }

        public:
          draw_method method;
          GLenum mode;
          size_t count;
          GLenum type;
          size_t start_index;
          size_t prim_count;
      };
    } // namespace geometry
  } // namespace yaggler
} // namespace neam

#endif /*__N_18391655302072008314_1562393302__OPENGL_DRAW_STATE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

