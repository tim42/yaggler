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
      /// \brief This class describes interleaved components of a buffer.
      /// The GL concept behind this is the Vertex Array Attributes
      /// This class is to be used at compile time
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
          /// \brief constructor
          constexpr buffer_view() : _i(0)
          {
          }

          /// \brief return the attribute index of this view
          GLuint get_attribute_index() const
          {
            return Init::attribute_index;
          }

          /// \brief return the number of components of the view
          GLuint get_size() const
          {
            return Init::size;
          }

          /// \brief return the type of the data described by the view
          GLenum get_type() const
          {
            return Init::type;
          }

          /// \brief return whether the data have to be normalized
          auto get_normalized() const
          -> NCR_ENABLE_IF((DestPrecision::value == destination_precision::single_precision), bool)
          {
            return Init::normalized == GL_TRUE;
          }

          /// \brief return the stride parameter
          GLuint get_stride() const
          {
            return Init::stride;
          }

          /// \brief return the offset parameter
          GLuint get_offset() const
          {
            return Init::offset;
          }

          /// \brief Use/Enable the vertex array attribute
          void use() const
          {
            _sub_use(0);
          }

          /// \brief disable the vertex array attribute
          void unuse() const
          {
            glDisableVertexAttribArray(Init::attribute_index);
          }

          int _i; ///< \brief unused, just to have something in here
      };

      /// \brief This class describes interleaved components of a buffer.
      /// The GL concept behind this is the Vertex Array Attributes
      /// This class is to be used at run time
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
          /// \brief construct a new buffer view by initializing only its attribute property
          /// \param _attribute the new attribute index
          constexpr buffer_view(GLuint _attribute)
            : attribute(_attribute)
          {
          }

          /// \brief construct and initialize all the properties of the buffer view
          /// \param _attribute the new attribute index
          /// \param _size the new number of components
          /// \param _type the new type of the components
          /// \param _stride the new stride to set
          /// \param _offset the new offset to set
          /// \param _normalized Whether or not the data have to be normalized
          buffer_view(GLuint _attribute, GLuint _size, GLenum _type, GLuint _stride, GLuint _offset, bool _normalized = false)
            : attribute(_attribute), size(_size), type(_type), stride(_stride), offset(_offset), normalized(_normalized ? GL_TRUE : GL_FALSE)
          {
          }

          /// \brief destructor
          ~buffer_view()
          {
          }

          /// \brief return the attribute index of this view
          GLuint get_attribute_index() const
          {
            return attribute;
          }

          /// \brief return the number of components of the view
          GLuint get_size() const
          {
            return size;
          }

          /// \brief return the type of the data described by the view
          GLenum get_type() const
          {
            return type;
          }

          /// \brief return whether the data have to be normalized
          auto get_normalized() const
          -> NCR_ENABLE_IF((DestPrecision::value == destination_precision::single_precision), bool)
          {
            return normalized == GL_TRUE;
          }

          /// \brief return the stride parameter
          GLuint get_stride() const
          {
            return stride;
          }

          /// \brief return the offset parameter
          GLuint get_offset() const
          {
            return offset;
          }

          /// \brief return the attribute index of this view
          /// \param _attribute the new attribute index
          void set_attribute_index(GLuint _attribute)
          {
            attribute = _attribute;
          }

          /// \brief set the number of components of the view
          /// \param _size the new number of components
          void set_size(GLuint _size)
          {
            size = _size;
          }

          /// \brief set the type of the data described by the view
          /// \param _type the new type of the components
          void set_type(GLenum _type)
          {
            type = _type;
          }

          /// \brief set whether the data have to be normalized
          /// \param _normalized Whether or not the data have to be normalized
          auto set_normalized(bool _normalized)
          -> NCR_ENABLE_IF((DestPrecision::value == destination_precision::single_precision), void)
          {
            normalized = _normalized ? GL_TRUE : GL_FALSE;
          }

          /// \brief set the stride parameter
          /// \param _stride the new stride to set
          void set_stride(GLuint _stride)
          {
            stride = _stride;
          }

          /// \brief set the offset parameter
          /// \param _offset the new offset to set
          void set_offset(GLuint _offset)
          {
            offset = _offset;
          }

          /// \brief set everything in one call
          /// \param _size the new number of components
          /// \param _type the new type of the components
          /// \param _stride the new stride to set
          /// \param _offset the new offset to set
          /// \param _normalized Whether or not the data have to be normalized
          auto set_parameters(GLuint _size, GLenum _type, GLuint _stride, GLuint _offset, bool _normalized)
          -> NCR_ENABLE_IF((DestPrecision::value == destination_precision::single_precision), void)
          {
            size = _size;
            type = _type;
            stride = _stride;
            offset = _offset;
            normalized = _normalized ? GL_TRUE : GL_FALSE;
          }

          /// \brief set everything in one call
          /// \param _size the new number of components
          /// \param _type the new type of the components
          /// \param _stride the new stride to set
          /// \param _offset the new offset to set
          auto set_parameters(GLuint _size, GLenum _type, GLuint _stride, GLuint _offset)
          -> NCR_ENABLE_IF((DestPrecision::value != destination_precision::single_precision), void)
          {
            size = _size;
            type = _type;
            stride = _stride;
            offset = _offset;
          }

          /// \brief Use/Enable the vertex array attribute
          void use() const
          {
            _sub_use(0);
          }

          /// \brief disable the vertex array attribute
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

