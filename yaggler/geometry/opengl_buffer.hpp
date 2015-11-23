//
// file : opengl_object.hpp
// in : file:///home/tim/projects/yaggler/yaggler/geometry/opengl_buffer.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 18/01/2014 12:18:58
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


#ifndef __N_19241511841048067712_64997576__OPENGL_BUFFER_HPP__
# define __N_19241511841048067712_64997576__OPENGL_BUFFER_HPP__

#include <yaggler_type.hpp>

#include <GLEW/glew.h>

#include <geometry/buffer_base.hpp>
#include <geometry/options.hpp>
#include <tools/array_wrapper.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace geometry
    {
      /// \brief This is an openGL buffer
      /// \param GeomType is an GLenum embed.
      ///
      /// \param Args... should be used to init "at ct" (in fact, thre's also a bit at runtime) the buffer.
      template<typename GeomType, typename... Args>
      class buffer<type::opengl, GeomType, Args...>
      {
        static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "the number of tpl argument after the geometry type should be 0 or 1 (see geometry::options::ct_buffer_init)");

        private:
          // init/no convert
          template<typename Init>
          void __split_init(NCR_ENABLE_IF((Init::convert == false), size_t))
          {
            Init init;
            set_data(init.data, init.draw_type);
          }

          // init/convert
          template<typename Init>
          void __split_init(NCR_ENABLE_IF((Init::convert == true), size_t))
          {
            Init init;
            set_data_convert_to_float(init.data, init.draw_type);
          }

          // no init
          template<size_t = 0>
          void __split_init(size_t)
          {
          }

          void __tpl_init()
          {
            __split_init<Args...>(0);
          }

          void DSA_aware_glBufferData(GLuint id, GLenum target, GLsizei size, const void *data, GLenum usage)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glNamedBufferDataEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glNamedBufferDataEXT(id, size, data, usage);
            }
            else
            {
              bind();
              glBufferData(target, size, data, usage);
            }
          }

#undef Y_DSA_COND
          void DSA_aware_glBufferSubData(GLuint id, GLenum target, GLintptr offset, GLsizei size, const void *data)
          {
#ifdef YAGGLER_USE_DSA_IF_AVAILABLE
#define Y_DSA_COND  (!!glNamedBufferSubDataEXT)
#else
#define Y_DSA_COND  true
#endif
            if (::opengl_version::useDSA && Y_DSA_COND) // NOTE: as useDSA is constexpr, the dead-code elimination pass will remove that branching
            {
              glNamedBufferSubDataEXT(id, offset, size, data);
            }
            else
            {
              bind();
              glBufferSubData(target, offset, size, data);
            }
          }
#undef Y_DSA_COND

        public:
          /// \brief Allocate a new buffer and acquire its ownership
          buffer()
          : id(0), ownership(true)
          {
            glGenBuffers(1, &id);
            __tpl_init();
          }

          /// \brief Create a wrapper linked to an openGL buffer ID, allowing the GL buffer object to be managed for a short time
          /// by this class. When the created instance has ended its life, the GL object won't be destructed
          /// \param _id the openGL buffer object ID
          explicit buffer(GLuint _id, size_t _binding_index = 0)
            : id(_id), ownership(false), binding_index(_binding_index)
          {
            __tpl_init();
          }

          /// \brief Create a wrapper of an openGL buffer ID, allowing the GL buffer object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed
          /// \param _id the openGL buffer object ID
          buffer(GLuint _id, assume_ownership_t)
            : id(_id), ownership(true)
          {
            __tpl_init();
          }

          /// \brief Create a wrapper of an openGL buffer ID, allowing the GL buffer object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed
          /// \param _id the openGL buffer object ID
          buffer(GLuint _id, size_t _binding_index, assume_ownership_t)
            : id(_id), ownership(true), binding_index(_binding_index)
          {
            __tpl_init();
          }
          /// \brief create a wrapper of the same GL buffer object in another buffer instance, allowing the GL buffer object to be managed for a short time
          /// by this class. When the created instance has ended its life, the GL object won't be destructed
          /// \param b the other buffer class to copy the information from
          template<typename... BArgs>
          buffer(const buffer<type::opengl, GeomType, BArgs...> &b)
          : id(b.get_id()), ownership(false), binding_index(b.get_binding_point())
          {
            __tpl_init();
          }

          /// \brief create a wrapper of the same GL buffer object in another buffer instance, allowing the GL buffer object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p b HAS THE OWNERSHIP)
          /// \param b the other buffer class to copy the information from
          template<typename... BArgs>
          buffer(buffer<type::opengl, GeomType, BArgs...> &&b)
          : id(b.get_id()), ownership(true), binding_index(b.get_binding_point())
          {
            b.give_up_ownership();
            __tpl_init();
          }

          /// \brief create a wrapper of the same GL buffer object in another buffer instance, allowing the GL buffer object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p b HAS THE OWNERSHIP)
          /// \param b the other buffer class to copy the information from
          template<typename... BArgs>
          buffer(buffer<type::opengl, GeomType, BArgs...> &b, stole_ownership_t)
          : id(b.get_id()), ownership(true), binding_index(b.get_binding_point())
          {
            b.give_up_ownership();
            __tpl_init();
          }

          /// \brief destructor
          ~buffer()
          {
            if (ownership)
            {
              glDeleteBuffers(1, &id);
            }
          }

          /// \brief give up the buffer GL object ownership WITHOUT DELETING IT
          /// (When the buffer<> instance has ended its life, the GL object won't be destructed)
          buffer &give_up_ownership()
          {
            ownership = false;
            return *this;
          }

          /// \brief assume the buffer GL object ownership
          /// (When the buffer<> instance has ended its life, the GL object WILL be destructed)
          buffer &assume_ownership()
          {
            ownership = true;
            return *this;
          }

          /// \brief create a wrapper of the same GL buffer object in another buffer instance, allowing the GL buffer object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p b HAS THE OWNERSHIP)
          /// \param b the other buffer class to copy the information from
          template<typename... BArgs>
          buffer &stole(buffer<type::opengl, GeomType, BArgs...> &b)
          {
            if (&b != this)
            {
              if (ownership)
                glDeleteBuffers(1, &id);

              ownership = b.has_ownership();
              id = b.get_id();
              binding_index = b.get_binding_point();
              b.give_up_ownership();
            }
            return *this;
          }

          /// \brief create a wrapper of the same GL buffer object in another buffer instance, allowing the GL buffer object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p b HAS THE OWNERSHIP)
          /// \param b the other buffer class to copy the information from
          template<typename... BArgs>
          buffer &link_to(const buffer<type::opengl, GeomType, BArgs...> &b)
          {
            if (&b != this)
            {
              if (ownership)
                glDeleteBuffers(1, &id);

              ownership = false;
              id = b.get_id();
              binding_index = b.get_binding_point();
            }
            return *this;
          }

          // this won't do what you want.
          buffer &operator = (const buffer &) = delete;

          /// \brief return the GL object id for the buffer
          GLuint get_id() const
          {
            return id;
          }

          /// \brief return true if the instance has the ownership of the GL object, false otherwise
          bool has_ownership() const
          {
            return ownership;
          }

          /// \brief bind the buffer
          void bind() const
          {
            glBindBuffer(GeomType::value, id);
          }

          /// \brief Bind the buffer to a different buffer target
          void bind_to_target(GLenum value) const
          {
            glBindBuffer(value, id);
          }

          /// \brief bind the buffer
          void bind_range(GLuint index, size_t offset, size_t size) const
          {
            glBindBufferRange(GeomType::value, index, id, offset, size);
          }

          /// \brief Bind the buffer to a different buffer target
          void bind_range_to_target(GLenum value, GLuint index, size_t offset, size_t size) const
          {
            glBindBufferRange(value, index, id, offset, size);
          }

          /// \brief Unbind a target
          void unbind_target(GLenum value) const
          {
            glBindBuffer(value, 0);
          }

          /// \brief unbind the buffer
          void unbind() const
          {
            glBindBuffer(GeomType::value, 0);
          }

          /// \brief _use_ the buffer, binding the buffer to its binding_index if applicable
          /// \see set_binding_point()
          void use() const
          {
            if (GeomType::value == GL_UNIFORM_BUFFER || GeomType::value == GL_ATOMIC_COUNTER_BUFFER || GeomType::value == GL_SHADER_STORAGE_BUFFER  || GeomType::value == GL_TRANSFORM_FEEDBACK_BUFFER)
              glBindBufferBase(GeomType::value, binding_index, id);
            else
              glBindBuffer(GeomType::value, id);
          }

          /// \brief un-use the buffer, unbinding it from its binding point
          void unuse() const
          {
            if (GeomType::value == GL_UNIFORM_BUFFER || GeomType::value == GL_ATOMIC_COUNTER_BUFFER || GeomType::value == GL_SHADER_STORAGE_BUFFER  || GeomType::value == GL_TRANSFORM_FEEDBACK_BUFFER)
              glBindBufferBase(GeomType::value, binding_index, 0);
            else
              glBindBuffer(GeomType::value, 0);
          }


          /// \brief set the binding index of the buffer
          /// \param index the binding index of the buffer
          void set_binding_point(GLuint index)
          {
            binding_index = index;
          }

          /// \brief get the binding point of the buffer
          GLuint get_binding_point() const
          {
            return binding_index;
          }


          /// \brief set the data from a C array (with fixed size)
          /// \param data the data to set
          /// \param draw_type a hint for openGL to choose the memory where the buffer will be stored
          template<typename ArrayType, size_t ArraySize>
          void set_data(const ArrayType (&data)[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
          {
            DSA_aware_glBufferData(id, GeomType::value, sizeof(ArrayType) * ArraySize, data, draw_type);
          }

          /// \brief set the data from an array_wrapper
          /// \param data the data to set
          /// \param draw_type a hint for openGL to choose the memory where the buffer will be stored
          template<typename ArrayType>
          void set_data(const array_wrapper<ArrayType> &w, GLenum draw_type = GL_STATIC_DRAW)
          {
            DSA_aware_glBufferData(id, GeomType::value, (sizeof(ArrayType) * w.size), w.array, draw_type);
          }

          /// \brief set the data from an C array (with fixed size)
          /// \param data the data to set
          /// \param draw_type a hint for openGL to choose the memory where the buffer will be stored
          template<size_t ArraySize>
          void set_data_convert_to_float(ct::vector4 data[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
          {
            GLfloat *dest_data = new GLfloat[ArraySize * 4];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = ct::conversion::to<GLfloat>(data[i].x);
              dest_data[i + 1] = ct::conversion::to<GLfloat>(data[i].y);
              dest_data[i + 2] = ct::conversion::to<GLfloat>(data[i].z);
              dest_data[i + 3] = ct::conversion::to<GLfloat>(data[i].w);
            }

            DSA_aware_glBufferData(id, GeomType::value, sizeof(GLfloat) * 4 * ArraySize, dest_data, draw_type);
            delete [] dest_data;
          }

          /// \brief set the data from an C array (with fixed size)
          /// \param data the data to set
          /// \param draw_type a hint for openGL to choose the memory where the buffer will be stored
          template<size_t ArraySize>
          void set_data_convert_to_float(ct::vector3 data[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
          {
            GLfloat *dest_data = new GLfloat[ArraySize * 3];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = ct::conversion::to<GLfloat>(data[i].x);
              dest_data[i + 1] = ct::conversion::to<GLfloat>(data[i].y);
              dest_data[i + 2] = ct::conversion::to<GLfloat>(data[i].z);
            }

            DSA_aware_glBufferData(id, GeomType::value, sizeof(GLfloat) * 3 * ArraySize, dest_data, draw_type);
            delete [] dest_data;
          }

          /// \brief set the data from an C array (with fixed size)
          /// \param data the data to set
          /// \param draw_type a hint for openGL to choose the memory where the buffer will be stored
          template<size_t ArraySize>
          void set_data_convert_to_float(ct::vector2 data[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
          {
            GLfloat *dest_data = new GLfloat[ArraySize * 2];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = ct::conversion::to<GLfloat>(data[i].x);
              dest_data[i + 1] = ct::conversion::to<GLfloat>(data[i].y);
            }

            DSA_aware_glBufferData(id, GeomType::value, sizeof(GLfloat) * 2 * ArraySize, dest_data, draw_type);
            delete [] dest_data;
          }

          /// \brief set the data from an C array (with fixed size)
          /// \param data the data to set
          /// \param draw_type a hint for openGL to choose the memory where the buffer will be stored
          template<size_t ArraySize>
          void set_data_convert_to_float(ct::fixed_t data[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
          {
            GLfloat *dest_data = new GLfloat[ArraySize];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = ct::conversion::to<GLfloat>(data[i]);
            }

            DSA_aware_glBufferData(id, GeomType::value, sizeof(GLfloat) * ArraySize, dest_data, draw_type);
            delete [] dest_data;
          }

          /// \brief set the data from a C array (with fixed size)
          /// \param data the data to set
          /// \param offset The position/offset where the data will be written
          template<typename ArrayType, size_t ArraySize>
          void set_subdata(const ArrayType (&data)[ArraySize], size_t offset)
          {
            DSA_aware_glBufferSubData(id, GeomType::value, offset, sizeof(ArrayType) * ArraySize, data);
          }

          /// \brief set the data from an array_wrapper
          /// \param data the data to set
          /// \param offset The position/offset where the data will be written
          template<typename ArrayType>
          void set_subdata(const array_wrapper<ArrayType> &w, size_t offset)
          {
            DSA_aware_glBufferSubData(id, GeomType::value, offset, (sizeof(ArrayType) * w.size), w.array);
          }

          /// \brief set the data from an C array (with fixed size)
          /// \param data the data to set
          /// \param offset The position/offset where the data will be written
          template<size_t ArraySize>
          void set_subdata_convert_to_float(ct::vector4 data[ArraySize], size_t offset)
          {
            GLfloat *dest_data = new GLfloat[ArraySize * 4];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = ct::conversion::to<GLfloat>(data[i].x);
              dest_data[i + 1] = ct::conversion::to<GLfloat>(data[i].y);
              dest_data[i + 2] = ct::conversion::to<GLfloat>(data[i].z);
              dest_data[i + 3] = ct::conversion::to<GLfloat>(data[i].w);
            }

            DSA_aware_glBufferSubData(id, GeomType::value, offset, sizeof(GLfloat) * 4 * ArraySize, dest_data);
            delete [] dest_data;
          }

          /// \brief set the data from an C array (with fixed size)
          /// \param data the data to set
          /// \param offset The position/offset where the data will be written
          template<size_t ArraySize>
          void set_subdata_convert_to_float(ct::vector3 data[ArraySize], size_t offset)
          {
            GLfloat *dest_data = new GLfloat[ArraySize * 3];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = ct::conversion::to<GLfloat>(data[i].x);
              dest_data[i + 1] = ct::conversion::to<GLfloat>(data[i].y);
              dest_data[i + 2] = ct::conversion::to<GLfloat>(data[i].z);
            }

            DSA_aware_glBufferSubData(id, GeomType::value, offset, sizeof(GLfloat) * 3 * ArraySize, dest_data);
            delete [] dest_data;
          }

          /// \brief set the data from an C array (with fixed size)
          /// \param data the data to set
          /// \param offset The position/offset where the data will be written
          template<size_t ArraySize>
          void set_subdata_convert_to_float(ct::vector2 data[ArraySize], size_t offset)
          {
            GLfloat *dest_data = new GLfloat[ArraySize * 2];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = ct::conversion::to<GLfloat>(data[i].x);
              dest_data[i + 1] = ct::conversion::to<GLfloat>(data[i].y);
            }

            DSA_aware_glBufferSubData(id, GeomType::value, offset, sizeof(GLfloat) * 2 * ArraySize, dest_data);
            delete [] dest_data;
          }

          /// \brief set the data from an C array (with fixed size)
          /// \param data the data to set
          /// \param offset The position/offset where the data will be written
          template<size_t ArraySize>
          void set_subdata_convert_to_float(ct::fixed_t data[ArraySize], size_t offset)
          {
            GLfloat *dest_data = new GLfloat[ArraySize];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = ct::conversion::to<GLfloat>(data[i]);
            }

            DSA_aware_glBufferSubData(id, GeomType::value, offset, sizeof(GLfloat) * ArraySize, dest_data);
            delete [] dest_data;
          }

          /// \brief Change the buffer type
          /// \note the returned object does not have the ownership on the GL resource, but shares the same default binding index
          template<GLenum MutateType>
          buffer<type::opengl, embed::GLenum<MutateType>> mutate_buffer() const
          {
            return buffer<type::opengl, embed::GLenum<MutateType>>(id, binding_index);
          }

          /// \brief Change the buffer type
          /// \note the returned object will have the ownership on the GL resource if the current buffer has its ownership
          template<GLenum MutateType>
          buffer<type::opengl, embed::GLenum<MutateType>> mutate_and_stole_buffer()
          {
            if (has_ownership())
            {
              give_up_ownership();
              return buffer<type::opengl, embed::GLenum<MutateType>>(id, binding_index, neam::assume_ownership);
            }
            else
              return buffer<type::opengl, embed::GLenum<MutateType>>(id, binding_index);
          }

          /// \brief create a link to a more generic buffer (generic when speaking of C++ types).
          /// no inheritance involved. This cast will create a 'link' buffer object.
          operator buffer<type::opengl, GeomType> ()
          {
            return buffer<type::opengl, GeomType> (*this);
          }

        public:
          static constexpr GLenum geom_type = GeomType::get();

        private:
          GLuint id;
          bool ownership;
          GLuint binding_index = 0;
      };
    } // namespace geometry
  } // namespace yaggler
} // namespace neam

#endif /*__N_19241511841048067712_64997576__OPENGL_BUFFER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

