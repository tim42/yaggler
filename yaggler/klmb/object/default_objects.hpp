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
        // [-1..1] quad
        GLfloat fs_quad [] __attribute__((unused)) =
        {
          -1.0f, -1.f, 0.0f,
          1.0f, -1.0f, 0.0f,
          -1.0f,  1.0f, 0.0f,
          -1.0f,  1.0f, 0.0f,
          1.0f, -1.0f, 0.0f,
          1.0f,  1.0f, 0.0f,
        };

        // [0..1] quad
        GLfloat positive_quad [] __attribute__((unused)) =
        {
          0.0f, 0.f, 0.0f,
          1.0f, 0.0f, 0.0f,
          0.0f,  1.0f, 0.0f,
          0.0f,  1.0f, 0.0f,
          1.0f, 0.0f, 0.0f,
          1.0f,  1.0f, 0.0f,
        };
      } // namespace object_data

      // non indexed data
      template<size_t Size>
      object<GL_ARRAY_BUFFER> create_object_from_object_data(const GLfloat (&data)[Size])
      {
        object<GL_ARRAY_BUFFER> obj;

        for (size_t i = 0; i < Size / 3; ++i)
          obj.bounding_box.add_vertex(glm::vec3(data[i * 3 + 0], data[i * 3 + 1], data[i * 3 + 2]));

        obj.drawer.set_draw_method(neam::yaggler::geometry::draw_method::normal);
        obj.drawer.set_draw_triangles(Size / 3);

        obj.ct_buffers.get<0>().set_data(data);

        obj.vao.add_buffer(obj.ct_buffers.get<0>(), neam::yaggler::geometry::buffer_view<neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>>
                              (0, 3, GL_FLOAT, 0, 0));

        return obj;
      }

      // indexed data
      template<size_t VertexArraySize, size_t IndexArraySize>
      object<GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER> create_object_from_object_data(const GLfloat (&vertex)[VertexArraySize], const GLuint (&indices)[IndexArraySize])
      {
        object<GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER> obj;

        for (size_t i = 0; i < VertexArraySize / 3; ++i)
          obj.bounding_box.add_vertex(glm::vec3(vertex[i * 3 + 0], vertex[i * 3 + 1], vertex[i * 3 + 2]));

        obj.drawer.set_draw_method(neam::yaggler::geometry::draw_method::indexed);
        obj.drawer.set_draw_triangles(VertexArraySize / 3);
        obj.drawer.set_index_type(GL_UNSIGNED_INT);

        obj.ct_buffers.get<0>().set_data(vertex);
        obj.ct_buffers.get<1>().set_data(indices);

        obj.vao.add_buffer(obj.ct_buffers.get<0>(), neam::yaggler::geometry::buffer_view<neam::yaggler::type::opengl, neam::embed::geometry::destination_precision<neam::yaggler::geometry::destination_precision::single_precision>>
                              (0, 3, GL_FLOAT, 0, 0));

        obj.vao.add_buffer(obj.ct_buffers.get<1>());

        return obj;
      }


    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_7729963021964170733_1262846684__DEFAULT_OBJECTS_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

