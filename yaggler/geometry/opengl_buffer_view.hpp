//
// file : opengl_buffer_view.hpp
// in : file:///home/tim/projects/yaggler/yaggler/geometry/opengl_buffer_view.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 20/01/2014 16:05:05
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


#ifndef __N_1765432327312661806_543905733__OPENGL_BUFFER_VIEW_HPP__
# define __N_1765432327312661806_543905733__OPENGL_BUFFER_VIEW_HPP__

#include <GLEW/glew.h>
#include <yaggler_type.hpp>

#include "geometry/buffer_view_base.hpp"
#include "options.hpp"

namespace neam
{
  namespace yaggler
  {
    namespace geometry
    {
      // this class doesn't represent an opengl object/concept, but something
      // tied with rendering parts/* of a buffer object
      //
      // this act much like a VAO, but without any optimization.
      //
      // This is the CT class.
      template<typename DestPrecision, typename Init>
      class buffer_view<type::opengl, DestPrecision, Init>
      {
        private: // helpers
          // sub use
          void _sub_use(NCR_ENABLE_IF((DestPrecision::value == destination_precision::single_precision), int)) const
          {
            glEnableVertexAttribArray(Init::attribute_index);
            glVertexAttribPointer(Init::attribute_index, Init::size, Init::type, Init::normalized, Init::stride, reinterpret_cast<GLvoid *>(Init::offset));
          }

          void _sub_use(NCR_ENABLE_IF((DestPrecision::value == destination_precision::double_precision), int)) const
          {
            glEnableVertexAttribArray(Init::attribute_index);
            glVertexAttribLPointer(Init::attribute_index, Init::size, Init::type, Init::stride, reinterpret_cast<GLvoid *>(Init::offset));
          }

          void _sub_use(NCR_ENABLE_IF((DestPrecision::value == destination_precision::integer), int)) const
          {
            glEnableVertexAttribArray(Init::attribute_index);
            glVertexAttribIPointer(Init::attribute_index, Init::size, Init::type, Init::stride, reinterpret_cast<GLvoid *>(Init::offset));
          }

        public:
          constexpr buffer_view()
          {
          }

          // getters
          GLuint get_attribute_index() const
          {
            return Init::attribute_index;
          }

          GLuint get_size() const
          {
            return Init::size;
          }

          GLenum get_type() const
          {
            return Init::type;
          }

          auto get_normalized() const
          -> NCR_ENABLE_IF((DestPrecision::value == destination_precision::single_precision), GLint)
          {
            return Init::normalized;
          }

          GLuint get_stride() const
          {
            return Init::stride;
          }

          GLuint get_offset() const
          {
            return Init::offset;
          }

          // use / unuse functions
          void use() const
          {
            _sub_use(0);
          }

          void unuse() const
          {
            glDisableVertexAttribArray(Init::attribute_index);
          }
      };

      // this class doesn't represent an opengl object/concept, but something
      // tied with rendering parts/* of a buffer object
      //
      // this act much like a VAO, but without any optimization.
      //
      // This is the runtime classb
      template<typename DestPrecision>
      class buffer_view<type::opengl, DestPrecision>
      {
        private: // helpers
          // sub use
          void _sub_use(NCR_ENABLE_IF((DestPrecision::value == destination_precision::single_precision), int)) const
          {
            glEnableVertexAttribArray(attribute);
            glVertexAttribPointer(attribute, size, type, normalized, stride, reinterpret_cast<GLvoid *>(offset));
          }

          void _sub_use(NCR_ENABLE_IF((DestPrecision::value == destination_precision::double_precision), int)) const
          {
            glEnableVertexAttribArray(attribute);
            glVertexAttribLPointer(attribute, size, type, stride, reinterpret_cast<GLvoid *>(offset));
          }

          void _sub_use(NCR_ENABLE_IF((DestPrecision::value == destination_precision::integer), int)) const
          {
            glEnableVertexAttribArray(attribute);
            glVertexAttribIPointer(attribute, size, type, stride, reinterpret_cast<GLvoid *>(offset));
          }

        public:
          buffer_view(GLuint _attribute)
            : attribute(_attribute)
          {
          }

          buffer_view(GLuint _attribute, GLuint _size, GLenum _type, GLuint _stride, GLuint _offset, GLint _normalized = false)
            : attribute(_attribute), size(_size), type(_type), stride(_stride), offset(_offset), normalized(_normalized)
          {
          }

          ~buffer_view()
          {
          }

          // getters
          GLuint get_attribute_index() const
          {
            return attribute;
          }

          GLuint get_size() const
          {
            return size;
          }

          GLenum get_type() const
          {
            return type;
          }

          auto get_normalized() const
          -> NCR_ENABLE_IF((DestPrecision::value == destination_precision::single_precision), GLint)
          {
            return normalized;
          }

          GLuint get_stride() const
          {
            return stride;
          }

          GLuint get_offset() const
          {
            return offset;
          }

          // setters
          void set_attribute_index(GLuint _attribute)
          {
            attribute = _attribute;
          }

          void set_size(GLuint _size)
          {
            size = _size;
          }

          void set_type(GLenum _type)
          {
            type = _type;
          }

          auto set_normalized(GLint _normalized)
          -> NCR_ENABLE_IF((DestPrecision::value == destination_precision::single_precision), void)
          {
            normalized = _normalized;
          }

          void set_stride(GLuint _stride)
          {
            stride = _stride;
          }

          void set_offset(GLuint _offset)
          {
            offset = _offset;
          }

          // set everything in one call
          auto set_parameters(GLuint _size, GLenum _type, GLuint _stride, GLuint _offset, GLint _normalized)
          -> NCR_ENABLE_IF((DestPrecision::value == destination_precision::single_precision), void)
          {
            size = _size;
            type = _type;
            stride = _stride;
            offset = _offset;
            normalized = _normalized;
          }

          auto set_parameters(GLuint _size, GLenum _type, GLuint _stride, GLuint _offset)
          -> NCR_ENABLE_IF((DestPrecision::value != destination_precision::single_precision), void)
          {
            size = _size;
            type = _type;
            stride = _stride;
            offset = _offset;
          }

          // use / unuse functions
          void use() const
          {
            _sub_use(0);
          }

          void unuse() const
          {
            glDisableVertexAttribArray(attribute);
          }

        private:
          GLuint attribute;

          GLuint size;
          GLenum type;
          GLuint stride;
          GLuint offset;
          GLint normalized;
      };
    } // namespace geometry
  } // namespace yaggler
} // namespace neam

#endif /*__N_1765432327312661806_543905733__OPENGL_BUFFER_VIEW_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

