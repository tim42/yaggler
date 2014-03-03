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
      // GeomType is an GLenum embed.
      //
      // Args... should be used to init "at ct" (in fact, also a bit at runtime) the buffer.
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

        public:
          // constructors
          buffer()
          : id(0), link(false)
          {
            glGenBuffers(1, &id);
            __tpl_init();
          }

          explicit buffer(GLuint _id)
            : id(_id), link(true)
          {
            __tpl_init();
          }

          buffer(GLuint _id, assume_ownership_t)
            : id(_id), link(false)
          {
            __tpl_init();
          }

          template<typename... BArgs>
          buffer(const buffer<type::opengl, GeomType, BArgs...> &b)
          : id(b.get_id()), link(true)
          {
            __tpl_init();
          }

          template<typename... BArgs>
          buffer(buffer<type::opengl, GeomType, BArgs...> &&b)
          : id(b.get_id()), link(false)
          {
            b.give_up_ownership();
            __tpl_init();
          }

          template<typename... BArgs>
          buffer(buffer<type::opengl, GeomType, BArgs...> &b, stole_ownership_t)
          : id(b.get_id()), link(false)
          {
            b.give_up_ownership();
            __tpl_init();
          }

          // destructor
          ~buffer()
          {
            if (!link)
            {
              glDeleteBuffers(1, &id);
            }
          }

          // give up the buffer WITHOUT DELETING IT
          // (simply become a link)
          buffer &give_up_ownership()
          {
            link = true;
            return *this;
          }

          buffer &assume_ownership()
          {
            link = false;
            return *this;
          }

          // see stole_ownership_t
          template<typename... BArgs>
          buffer &stole(buffer<type::opengl, GeomType, BArgs...> &b)
          {
            if (&b != this)
            {
              if (!link)
                glDeleteBuffers(1, &id);

              link = b.is_link();
              id = b.get_id();
              b.give_up_ownership();
            }
            return *this;
          }

          // create a simple link
          template<typename... BArgs>
          buffer &link_to(const buffer<type::opengl, GeomType, BArgs...> &b)
          {
            if (&b != this)
            {
              if (!link)
                glDeleteBuffers(1, &id);

              link = true;
              id = b.get_id();
            }
            return *this;
          }

          // this won't do what you want.
          buffer &operator = (const buffer &) = delete;

          // getters
          GLuint get_id() const
          {
            return id;
          }

          bool is_link() const
          {
            return link;
          }

          // bind the buffer
          void bind() const
          {
            glBindBuffer(GeomType::value, id);
          }
          // bind the buffer
          void use() const
          {
            glBindBuffer(GeomType::value, id);
          }

          // set the data from a C array (with fixed size)
          template<typename ArrayType, size_t ArraySize>
          void set_data(const ArrayType (&data)[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
          {
            bind();
            glBufferData(GeomType::value, sizeof(data), data, draw_type);
          }
          // set the data from a C array (with fixed size)
//           template<typename ArrayType, size_t ArraySize>
//           void set_data(const ArrayType (&)[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
//           {
//             bind();
//             glBufferData(GeomType::value, sizeof(data), data, draw_type);
//           }
          // set the data from an array_wrapper
          template<typename ArrayType>
          void set_data(const array_wrapper<ArrayType> &w, GLenum draw_type = GL_STATIC_DRAW)
          {
            bind();
            glBufferData(GeomType::value, (sizeof(ArrayType) * w.size), w.array, draw_type);
          }

          void set_data(const array_wrapper<glm::vec4> &data, GLenum draw_type = GL_STATIC_DRAW)
          {
            bind();
            GLfloat *dest_data = new GLfloat[data.size * 4];
            for (size_t i = 0; i < data.size; ++i)
            {
              dest_data[i + 0] = (data.array[i].x);
              dest_data[i + 1] = (data.array[i].y);
              dest_data[i + 2] = (data.array[i].z);
              dest_data[i + 3] = (data.array[i].w);
            }

            glBufferData(GeomType::value, sizeof(GLfloat) * 4 * data.size, dest_data, draw_type);
            delete [] dest_data;
          }

          void set_data(const array_wrapper<glm::vec3> &data, GLenum draw_type = GL_STATIC_DRAW)
          {
            bind();
            GLfloat *dest_data = new GLfloat[data.size * 3];
            for (size_t i = 0; i < data.size; ++i)
            {
              dest_data[i * 3 + 0] = (data.array[i].x);
              dest_data[i * 3 + 1] = (data.array[i].y);
              dest_data[i * 3 + 2] = (data.array[i].z);
            }
            glBufferData(GeomType::value, sizeof(GLfloat) * 3 * data.size, dest_data, draw_type);
            delete [] dest_data;
          }
          void set_data(const array_wrapper<glm::vec2> &data, GLenum draw_type = GL_STATIC_DRAW)
          {
            bind();
            GLfloat *dest_data = new GLfloat[data.size * 2];
            for (size_t i = 0; i < data.size; ++i)
            {
              dest_data[i + 0] = (data.array[i].x);
              dest_data[i + 1] = (data.array[i].y);
            }
            glBufferData(GeomType::value, sizeof(GLfloat) * 2 * data.size, dest_data, draw_type);
            delete [] dest_data;
          }

          // set the data from an C array (with fixed size)
          template<size_t ArraySize>
          void set_data(glm::vec4 data[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
          {
            bind();
            GLfloat *dest_data = new GLfloat[ArraySize * 4];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = (data[i].x);
              dest_data[i + 1] = (data[i].y);
              dest_data[i + 2] = (data[i].z);
              dest_data[i + 3] = (data[i].w);
            }

            glBufferData(GeomType::value, sizeof(GLfloat) * 4 * ArraySize, dest_data, draw_type);
            delete [] dest_data;
          }
          // set the data from an C array (with fixed size)
          template<size_t ArraySize>
          void set_data(glm::vec3 data[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
          {
            bind();
            GLfloat *dest_data = new GLfloat[ArraySize * 3];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = (data[i].x);
              dest_data[i + 1] = (data[i].y);
              dest_data[i + 2] = (data[i].z);
            }

            glBufferData(GeomType::value, sizeof(GLfloat) * 3 * ArraySize, dest_data, draw_type);
            delete [] dest_data;
          }
          // set the data from an C array (with fixed size)
          template<size_t ArraySize>
          void set_data(glm::vec2 data[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
          {
            bind();
            GLfloat *dest_data = new GLfloat[ArraySize * 2];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = (data[i].x);
              dest_data[i + 1] = (data[i].y);
            }

            glBufferData(GeomType::value, sizeof(GLfloat) * 2 * ArraySize, dest_data, draw_type);
            delete [] dest_data;
          }

          // set the data from an C array (with fixed size)
          template<size_t ArraySize>
          void set_data_convert_to_float(ct::vector4 data[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
          {
            bind();
            GLfloat *dest_data = new GLfloat[ArraySize * 4];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = ct::conversion::to<GLfloat>(data[i].x);
              dest_data[i + 1] = ct::conversion::to<GLfloat>(data[i].y);
              dest_data[i + 2] = ct::conversion::to<GLfloat>(data[i].z);
              dest_data[i + 3] = ct::conversion::to<GLfloat>(data[i].w);
            }

            glBufferData(GeomType::value, sizeof(GLfloat) * 4 * ArraySize, dest_data, draw_type);
            delete [] dest_data;
          }
          // set the data from an C array (with fixed size)
          template<size_t ArraySize>
          void set_data_convert_to_float(ct::vector3 data[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
          {
            bind();
            GLfloat *dest_data = new GLfloat[ArraySize * 3];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = ct::conversion::to<GLfloat>(data[i].x);
              dest_data[i + 1] = ct::conversion::to<GLfloat>(data[i].y);
              dest_data[i + 2] = ct::conversion::to<GLfloat>(data[i].z);
            }

            glBufferData(GeomType::value, sizeof(GLfloat) * 3 * ArraySize, dest_data, draw_type);
            delete [] dest_data;
          }
          // set the data from an C array (with fixed size)
          template<size_t ArraySize>
          void set_data_convert_to_float(ct::vector2 data[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
          {
            bind();
            GLfloat *dest_data = new GLfloat[ArraySize * 2];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = ct::conversion::to<GLfloat>(data[i].x);
              dest_data[i + 1] = ct::conversion::to<GLfloat>(data[i].y);
            }

            glBufferData(GeomType::value, sizeof(GLfloat) * 2 * ArraySize, dest_data, draw_type);
            delete [] dest_data;
          }
          // set the data from an C array (with fixed size)
          template<size_t ArraySize>
          void set_data_convert_to_float(ct::fixed_t data[ArraySize], GLenum draw_type = GL_STATIC_DRAW)
          {
            bind();
            GLfloat *dest_data = new GLfloat[ArraySize];
            for (size_t i = 0; i < ArraySize; ++i)
            {
              dest_data[i + 0] = ct::conversion::to<GLfloat>(data[i]);
            }

            glBufferData(GeomType::value, sizeof(GLfloat) * ArraySize, dest_data, draw_type);
            delete [] dest_data;
          }

          // create a link to a more generic buffer.
          // no inheritance involved. This cast will create a 'link' program shader object.
          operator buffer<type::opengl, GeomType> ()
          {
            return buffer<type::opengl, GeomType> (*this);
          }

        public:
          static constexpr GLenum geom_type = GeomType::value;

        private:
          GLuint id;
          bool link;
      };
    } // namespace geometry
  } // namespace yaggler
} // namespace neam

#endif /*__N_19241511841048067712_64997576__OPENGL_BUFFER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

