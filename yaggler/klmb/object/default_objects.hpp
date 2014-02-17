//
// file : default_objects.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/object/default_objects.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 16/02/2014 14:31:23
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

#ifndef __N_7729963021964170733_1262846684__DEFAULT_OBJECTS_HPP__
# define __N_7729963021964170733_1262846684__DEFAULT_OBJECTS_HPP__

#include <klmb/object/object.hpp>
#include <geometry/buffer_view_base.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      namespace object_data
      {
        // The fullscreen quad
        GLfloat fs_quad [] =
        {
          -1.0f, -1.f, 0.0f,
          1.0f, -1.0f, 0.0f,
          -1.0f,  1.0f, 0.0f,
          -1.0f,  1.0f, 0.0f,
          1.0f, -1.0f, 0.0f,
          1.0f,  1.0f, 0.0f,
        };
      } // namespace internal

      // non indexed data
      template<size_t Size>
      object<GL_ARRAY_BUFFER> create_object_from_object_data(const GLfloat (&data)[Size])
      {
        object<GL_ARRAY_BUFFER> fsquad;

        fsquad.drawer.set_draw_method(neam::yaggler::geometry::draw_method::normal);
        fsquad.drawer.set_draw_triangles(Size / 3);

        fsquad.ct_buffers.get_ref<0>().set_data(data);

        fsquad.vao.add_buffer(fsquad.ct_buffers.get_ref<0>(), neam::yaggler::geometry::buffer_view<neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>>
                              (0, 3, GL_FLOAT, 0, 0));

        return fsquad;
      }

      // indexed data
      template<size_t VertexArraySize, size_t IndexArraySize>
      object<GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER> create_object_from_object_data(const GLfloat (&vertex)[VertexArraySize], const GLuint (&indices)[IndexArraySize])
      {
        object<GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER> fsquad;

        fsquad.drawer.set_draw_method(neam::yaggler::geometry::draw_method::indexed);
        fsquad.drawer.set_draw_triangles(VertexArraySize / 3);
        fsquad.drawer.set_index_type(GL_UNSIGNED_INT);

        fsquad.ct_buffers.get_ref<0>().set_data(vertex);
        fsquad.ct_buffers.get_ref<1>().set_data(indices);

        fsquad.vao.add_buffer(fsquad.ct_buffers.get_ref<0>(), neam::yaggler::geometry::buffer_view<neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>>
                              (0, 3, GL_FLOAT, 0, 0));

        fsquad.vao.add_buffer(fsquad.ct_buffers.get_ref<1>());

        return fsquad;
      }


    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_7729963021964170733_1262846684__DEFAULT_OBJECTS_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

