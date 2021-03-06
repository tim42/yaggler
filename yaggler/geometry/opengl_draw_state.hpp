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
#include <geometry/opengl_transform_feedback.hpp>
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
        indexed_instanced,

        transform_feedback,
        transform_feedback_instanced,
        transform_feedback_stream,
        transform_feedback_stream_instanced,

        indirect,
        multi_indirect,
        indexed_indirect,
        multi_indexed_indirect
      };

      /// \brief this class represent a draw state.
      /// (which kind of geometry will be draw, its size, ...)
      template<>
      class draw_state<type::opengl>
      {
        public:
          /// \brief initialize and create the draw state
          /// \param _method describe the kind of geometry to render
          draw_state(draw_method _method = draw_method::normal)
            : method(_method), mode(0), patch_size(0), count(0), type(GL_UNSIGNED_INT),
              start_index(0), prim_count(1), stream(0), stride(0), tf(0)
          {
          }

          /// \brief make the draw state render triangles
          /// \param triangle_count the number of triangle to draw
          void set_draw_triangles(size_t triangle_count)
          {
            mode = GL_TRIANGLES;
            count = triangle_count * 3;
          }

          /// \brief (de)activate and change the tesselation patch size
          /// \note use -1 to keep default (from last draw mode)
          void make_use_patches(int _patch_size = -1)
          {
            if (_patch_size < 0)
            {
              if (mode == GL_TRIANGLES || mode == 0)
                patch_size = 3;
              else if (mode == GL_LINE)
                patch_size = 2;
            }
            else
              patch_size = _patch_size;

#ifndef YAGGLER_NO_FUCKING_TESTS
            if (count % patch_size != 0)
              neam::cr::out.debug() << LOGGER_INFO << "make use patch: patch size (" << patch_size << ") is not a multiple of the number of vertices (" << count << ")" << std::endl;
#endif

            mode = GL_PATCHES;
          }

          /// \brief make the draw state render lines
          /// \param line_count the number of line to draw
          void set_draw_lines(size_t line_count)
          {
            mode = GL_LINES;
            count = line_count * 2;
          }

          /// \brief make the draw state render points
          /// \param line_count the number of point to draw
          void set_draw_points(size_t point_count)
          {
            mode = GL_POINTS;
            count = point_count;
          }

          /// \brief set a custom rendering mode with a custom geometry/element count
          void set_draw(GLenum _mode, size_t _count)
          {
            mode = _mode;
            count = _count;
          }

          /// \brief change the draw method
          /// \param _method describe the kind of geometry to render
          void set_draw_method(draw_method _method)
          {
            method = _method;
          }

          /// \brief return the draw method
          draw_method get_draw_method() const
          {
            return method;
          }

          /// \brief set the index type
          /// \note only used in indexed modes !
          /// \param _type the index type
          void set_index_type(GLenum _type)
          {
            type = _type;
          }

          /// \brief set the start index in the geometry buffer
          /// \note only used in normal and instanced modes
          /// \param _index the start index in the geometry buffer
          void set_start_index(size_t _index)
          {
            start_index = _index;
          }

          /// \brief set the start index in the indirect buffer
          /// \note only used in indirect modes
          /// \param _index the start index in the indirect buffer
          void set_indirect_buffer_offset(size_t _index)
          {
            start_index = _index;
          }

          /// \brief Specifie the distance in byte between elements of the indirect buffer
          /// \note only used in the mult-indirect mode
          /// \note the default value is 0, meaning the buffer is tightly packed.
          /// \link https://www.opengl.org/wiki/GLAPI/glMultiDrawArraysIndirect
          void set_stride(size_t _stride)
          {
            stride = _stride;
          }

          /// \brief set the primitive count
          /// \note only used in indexed* mode
          /// \param _prim_count the number of primitive to draw
          void set_primitive_count(size_t _prim_count)
          {
            prim_count = _prim_count;
          }

          /// \brief Set the stream to use
          /// \note only used by transform_feedback_stream*
          void set_stream(size_t _stream)
          {
            stream = _stream;
          }

          /// \brief Set the transform_feedback to use
          void set_transform_feedback(const transform_feedback<type::opengl> &_tf)
          {
            tf.link_to(_tf);
          }

          /// \brief Set the transform_feedback to use
          /// \note With this method, the internal transform_feedback object will have the ownership of the GL resource
          void set_transform_feedback(transform_feedback<type::opengl> &&_tf)
          {
            tf.stole(_tf);
          }

          /// \brief return the mode (which kind of geometry to draw)
          GLenum get_mode() const
          {
            return mode;
          }

          /// \brief return the number of geometry to draw
          size_t get_count() const
          {
            return count;
          }

          /// \brief return the index type
          GLenum get_index_type() const
          {
            return type;
          }

          /// \brief return the start index
          size_t get_start_index() const
          {
            return start_index;
          }

          /// \brief return the start index of the indirect buffer
          size_t get_indirect_buffer_offset() const
          {
            return start_index;
          }

          /// \brief return the distance in byte between elements of the indirect buffer
          size_t get_stride() const
          {
            return stride;
          }

          /// \brief return the number of primitive to draw
          size_t get_primitive_count() const
          {
            return prim_count;
          }

          /// \brief Get the stream to use
          size_t get_stream() const
          {
            return stream;
          }

          /// \brief Return the transform_feedback object
          transform_feedback<type::opengl> &get_transform_feedback()
          {
            return tf;
          }

          /// \brief Return the transform_feedback object
          const transform_feedback<type::opengl> &get_transform_feedback() const
          {
            return tf;
          }

          /// \brief do the draw call
          void draw() const
          {
            if (mode == GL_PATCHES)
              glPatchParameteri(GL_PATCH_VERTICES, patch_size);

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
              case draw_method::transform_feedback:
                glDrawTransformFeedback(mode, tf.get_id());
                break;
              case draw_method::transform_feedback_instanced:
                glDrawTransformFeedbackInstanced(mode, tf.get_id(), prim_count);
                break;
              case draw_method::transform_feedback_stream:
                glDrawTransformFeedbackStream(mode, tf.get_id(), stream);
                break;
              case draw_method::transform_feedback_stream_instanced:
                glDrawTransformFeedbackStreamInstanced(mode, tf.get_id(), stream, prim_count);
                break;
              case draw_method::indirect:
                glDrawArraysIndirect(mode, reinterpret_cast<GLvoid *>(start_index));
                break;
              case draw_method::multi_indirect:
                glMultiDrawArraysIndirect(mode, reinterpret_cast<GLvoid *>(start_index), count, stride);
                break;
              case draw_method::indexed_indirect:
                glDrawElementsIndirect(mode, type, reinterpret_cast<GLvoid *>(start_index));
                break;
              case draw_method::multi_indexed_indirect:
                glMultiDrawElementsIndirect(mode, type, reinterpret_cast<GLvoid *>(start_index), count, stride);
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
          size_t patch_size;
          size_t count;
          GLenum type;
          size_t start_index;
          size_t prim_count;
          size_t stream;
          size_t stride;
          transform_feedback<type::opengl> tf;
      };
    } // namespace geometry
  } // namespace yaggler
} // namespace neam

#endif /*__N_18391655302072008314_1562393302__OPENGL_DRAW_STATE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

