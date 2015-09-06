//
// file : opengl_object.hpp
// in : file:///home/tim/projects/yaggler/yaggler/geometry/opengl_vao.hpp
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


#ifndef __N_19241511841048067712_64997576__OPENGL_VAO_HPP__
# define __N_19241511841048067712_64997576__OPENGL_VAO_HPP__

#include <yaggler_type.hpp>
#include <GLEW/glew.h>

#include <tools/execute_pack.hpp>
#include <tools/tuple.hpp>
#include <geometry/vao_base.hpp>
#include <geometry/options.hpp>
#include <yaggler_except.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace geometry
    {
      /// \brief an openGL VAO
      template<typename... Init>
      class vao<type::opengl, Init...>
      {
        private: // helpers
          // init from <views/vbo>, <views/vbo>, ...
          template<size_t Idx, typename Opt>
          char __init_single(size_t)
          {
            ct_buffers_views.template get<Idx>().init();
            return 0;
          }

          template<size_t... Idxs>
          void __int_from_ct(neam::cr::seq<Idxs...>&&)
          {
            NEAM_EXECUTE_PACK((__init_single<Idxs, Init>(0)));
          }

        public:
          /// \brief Allocate a new vao and acquire its ownership
          vao()
          : id(0), ownership(true)
          {
            glGenVertexArrays(1, &id);
            bind();
            __int_from_ct(cr::gen_seq<sizeof...(Init)>());
          }

          /// \brief Create a wrapper linked to an openGL vao ID, allowing the GL vao object to be managed for a short time
          /// by this class. When the created instance has ended its life, the GL object won't be destructed
          /// \param _id the openGL vao object ID
          explicit vao(GLuint _id)
            : id(_id), ownership(false)
          {
            bind();
            __int_from_ct(cr::gen_seq<sizeof...(Init)>());
          }

          /// \brief Create a wrapper of an openGL vao ID, allowing the GL vao object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed
          /// \param _id the openGL vao object ID
          vao(GLuint _id, assume_ownership_t)
            : id(_id), ownership(true)
          {
            bind();
            __int_from_ct(cr::gen_seq<sizeof...(Init)>());
          }


          /// \brief create a wrapper of the same GL vao object in another vao instance, allowing the GL vao object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p o HAS THE OWNERSHIP)
          /// \param o the other vao class to copy the information from
          template<typename... OInit>
          vao(vao<type::opengl, OInit...> &o, stole_ownership_t)
            : id(o.get_id()), ownership(o.has_ownership())
          {
            o.give_up_ownership();
            bind();
            __int_from_ct(cr::gen_seq<sizeof...(Init)>());
          }

          /// \brief create a wrapper of the same GL vao object in another vao instance, allowing the GL vao object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p o HAS THE OWNERSHIP)
          /// \param o the other vao class to copy the information from
          template<typename... OInit>
          vao(vao<type::opengl, OInit...> && o)
            : id(o.get_id()), ownership(o.has_ownership())
          {
            o.give_up_ownership();
            bind();
            __int_from_ct(cr::gen_seq<sizeof...(Init)>());
          }

          /// \brief create a wrapper of the same GL vao object in another vao instance, allowing the GL vao object to be managed for a short time
          /// by this class. When the created instance has ended its life, the GL object won't be destructed
          /// \param b the other vao class to copy the information from
          template<typename... OInit>
          vao(const vao<type::opengl, OInit...> &o)
          : id(o.get_id()), ownership(false)
          {
            bind();
            __int_from_ct(cr::gen_seq<sizeof...(Init)>());
          }

          /// \brief destructor
          ~vao()
          {
            if (ownership)
              glDeleteVertexArrays(1, &id);
          }

          /// \brief give up the vao GL object ownership WITHOUT DELETING IT
          /// (When the vao<> instance has ended its life, the GL object won't be destructed)
          vao &give_up_ownership()
          {
            ownership = false;
            return *this;
          }

          /// \brief assume the vao GL object ownership
          /// (When the vao<> instance has ended its life, the GL object WILL be destructed)
          vao &assume_ownership()
          {
            ownership = true;
            return *this;
          }

          /// \brief create a wrapper of the same GL vao object in another vao instance, allowing the GL vao object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p o HAS THE OWNERSHIP)
          /// \param b the other vao class to copy the information from
          template<typename... OInit>
          vao &stole(vao<type::opengl, OInit...> &t)
          {
            if (&t != this)
            {
              if (ownership)
                glDeleteVertexArrays(1, &id);

              ownership = t.has_ownership();
              id = t.get_id();
              t.give_up_ownership();
            }
            return *this;
          }

          /// \brief create a wrapper of the same GL vao object in another vao instance, allowing the GL vao object to be managed
          /// by this class. When the created instance has ended its life, the GL object will be destructed (ONLY IF THE \p o HAS THE OWNERSHIP)
          /// \param b the other vao class to copy the information from
          template<typename... OInit>
          vao &link_to(const vao<type::opengl, OInit...> &t)
          {
            if (&t != this)
            {
              if (ownership)
                glDeleteVertexArrays(1, &id);

              ownership = false;
              id = t.get_id();
            }
            return *this;
          }

          /// \brief return the ID  of the underlying GL resource
          GLuint get_id() const
          {
            return id;
          }

          /// \brief return whether or not the current instance has the ownership of the underlying GL resource
          bool has_ownership() const
          {
            return ownership;
          }

          /// \brief bind the vao
          void bind() const
          {
            glBindVertexArray(id);
          }

          /// \brief bind the vao
          void use() const
          {
            glBindVertexArray(id);
          }

          /// \brief unbind the vao
          void unbind() const
          {
            glBindVertexArray(0);
          }

          /// \brief unbind the vao
          void unuse() const
          {
            glBindVertexArray(0);
          }

          /// \brief add a buffer and its views to the vao
          template<typename Buffer, typename... Views>
          void add_buffer(const Buffer &b, const Views &... vs)
          {
            use();
            b.use();
            NEAM_EXECUTE_PACK((vs.use()));
            unuse();

            b.unuse();
            NEAM_EXECUTE_PACK((vs.unuse()));
          }

          /// \brief create a link to a more generic vao.
          /// no inheritance is involved. This cast will create a 'link' program shader object.
          operator vao<type::opengl> ()
          {
            return vao<type::opengl> (*this);
          }

          /// \brief return a "link" to the buffer from the ct init at the specified index.
          template<size_t Idx, GLenum GeomType>
          buffer<type::opengl, neam::embed::GLenum<GeomType>> get_buffer_link()
          {
            return buffer<type::opengl, neam::embed::GLenum<GeomType>>(ct_buffers_views.template get<Idx>());
          }

          /// \brief return a reference to the buffer from the ct init at the specified index.
          template<size_t Idx>
          auto get_buffer() -> typename ct::type_at_index<Idx, Init...>::type &
          {
            return (ct_buffers_views.template get<Idx>());
          }

          /// \brief stole a buffer from the ct init at a specified index
          /// \see stole_ownership_t
          /// \attention COULD ONLY WORK ONE TIME for a given buffer (else throw)
          template<size_t Idx, GLenum GeomType>
          buffer<type::opengl, neam::embed::GLenum<GeomType>> stole_buffer()
          {
            if (ct_buffers_views.template get<Idx>().is_link())
              throw yaggler_exception("n/y::geometry::vao::stole_buffer(): unable to stole owneship: ownership has already been 'stolen'.");
            return buffer<type::opengl, neam::embed::GLenum<GeomType>>(ct_buffers_views.template get<Idx>(), stole_ownership);
          }

        private:
          GLuint id;
          bool ownership;

          // buffers/views couples
          cr::tuple<Init...> ct_buffers_views;
      };
    } // namespace geometry
  } // namespace yaggler
} // namespace neam

#endif /*__N_19241511841048067712_64997576__OPENGL_VAO_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

