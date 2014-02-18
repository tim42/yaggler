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

#include <geometry/vao_base.hpp>
#include <geometry/options.hpp>
#include <tools/tuple.hpp>
#include <yaggler_except.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace geometry
    {
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
            void((char []){__init_single<Idxs, Init>(0)...});
          }

        public:
          // constructors
          vao()
          : id(0), link(false)
          {
            glGenVertexArrays(1, &id);
            bind();
            __int_from_ct(cr::gen_seq<sizeof...(Init)>());
          }

          explicit vao(GLuint _id)
            : id(_id), link(true)
          {
            bind();
            __int_from_ct(cr::gen_seq<sizeof...(Init)>());
          }

          vao(GLuint _id, assume_ownership_t)
            : id(_id), link(false)
          {
            bind();
            __int_from_ct(cr::gen_seq<sizeof...(Init)>());
          }

          template<typename... OInit>
          vao(vao<type::opengl, OInit...> &o, stole_ownership_t)
            : id(o.get_id()), link(o.is_link())
          {
            o.give_up_ownership();
            bind();
            __int_from_ct(cr::gen_seq<sizeof...(Init)>());
          }
          template<typename... OInit>
          vao(vao<type::opengl, OInit...> && o)
            : id(o.get_id()), link(o.is_link())
          {
            o.give_up_ownership();
            bind();
            __int_from_ct(cr::gen_seq<sizeof...(Init)>());
          }

          template<typename... OInit>
          vao(const vao<type::opengl, OInit...> &o)
          : id(o.get_id()), link(true)
          {
            bind();
            __int_from_ct(cr::gen_seq<sizeof...(Init)>());
          }

          // destructor
          ~vao()
          {
            if (!link)
              glDeleteVertexArrays(1, &id);
          }

          // give up the buffer WITHOUT DELETING IT
          // (simply become a link)
          vao &give_up_ownership()
          {
            link = true;
            return *this;
          }

          vao &assume_ownership()
          {
            link = false;
            return *this;
          }

          // see stole_ownership_t
          template<typename... OInit>
          vao &stole(vao<type::opengl, OInit...> &t)
          {
            if (&t != this)
            {
              if (!link)
                glDeleteVertexArrays(1, &id);

              link = t.is_link();
              id = t.get_id();
              t.give_up_ownership();
            }
            return *this;
          }

          // create a simple link
          template<typename... OInit>
          vao &link_to(const vao<type::opengl, OInit...> &t)
          {
            if (&t != this)
            {
              if (!link)
                glDeleteVertexArrays(1, &id);

              link = true;
              id = t.get_id();
            }
            return *this;
          }

          // getters
          GLuint get_id() const
          {
            return id;
          }

          bool is_link() const
          {
            return link;
          }

          // bind the vao
          void bind() const
          {
            glBindVertexArray(id);
          }

          void use() const
          {
            glBindVertexArray(id);
          }

          // add buffer / views to the vao
          template<typename Buffer, typename... Views>
          void add_buffer(const Buffer &b, const Views &... vs)
          {
            use();
            b.use();
            void((int []){(vs.use(), 5)...}); // who knows how this'll be optimised out ?
            // (and which compiler supports it...)
          }

          // create a link to a more generic texture.
          // no inheritance involved. This cast will create a 'link' program shader object.
          operator vao<type::opengl> ()
          {
            return vao<type::opengl> (*this);
          }

          // YAY :D
          // (but there'll always be that annoying GeomType around... :( )
          //
          // return a "link" to the buffer at the specified index.
          // (buffers from the ct init)
          template<size_t Idx, GLenum GeomType>
          buffer<type::opengl, neam::embed::GLenum<GeomType>> get_buffer_link()
          {
            return buffer<type::opengl, neam::embed::GLenum<GeomType>>(ct_buffers_views.template get<Idx>());
          }

          // simply return a ref to the buffer
          template<size_t Idx>
          auto get_buffer() -> typename ct::type_at_index<Idx, Init...>::type &
          {
            return (ct_buffers_views.template get<Idx>());
          }

          // see stole_ownership_t
          // COULD ONLY WORK ONE TIME for a given buffer (else throw)
          template<size_t Idx, GLenum GeomType>
          buffer<type::opengl, neam::embed::GLenum<GeomType>> stole_buffer()
          {
            if (ct_buffers_views.template get<Idx>().is_link())
              throw yaggler_exception("n/y::geometry::vao::stole_buffer(): unable to stole owneship: ownership has already been 'stolen'.");
            return buffer<type::opengl, neam::embed::GLenum<GeomType>>(ct_buffers_views.template get<Idx>(), stole_ownership);
          }

        private:
          GLuint id;
          bool link;

          // buffers/views couples
          cr::tuple<Init...> ct_buffers_views;
      };
    } // namespace geometry
  } // namespace yaggler
} // namespace neam

#endif /*__N_19241511841048067712_64997576__OPENGL_VAO_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

