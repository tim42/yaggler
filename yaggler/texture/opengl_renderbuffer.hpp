//
// file : opengl_renderbuffer.hpp
// in : file:///home/tim/projects/yaggler/yaggler/texture/opengl_renderbuffer.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 21/02/2014 16:59:46
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

#ifndef __N_14995976871411965052_2102803391__OPENGL_RENDERBUFFER_HPP__
# define __N_14995976871411965052_2102803391__OPENGL_RENDERBUFFER_HPP__

#include <texture/renderbuffer_base.hpp>
#include <yaggler_type.hpp>
#include <GLEW/glew.h>

namespace neam
{
  namespace yaggler
  {
    namespace texture
    {
      template<>
      class renderbuffer<type::opengl>
      {
        public:
          renderbuffer()
            : id(0), link(false)
          {
            glGenRenderbuffers(1, &id);
          }

          renderbuffer(GLuint _id)
            : id(_id), link(true)
          {
          }

          renderbuffer(GLuint _id, assume_ownership_t)
            : id(_id), link(false)
          {
          }

          renderbuffer(const renderbuffer &rb)
            : id(rb.get_id()), link(true)
          {
          }

          renderbuffer(renderbuffer &&rb)
            : id(rb.get_id()), link(rb.is_link())
          {
            rb.give_up_ownership();
          }

          renderbuffer(renderbuffer &rb, stole_ownership_t)
            : id(rb.get_id()), link(rb.is_link())
          {
            rb.give_up_ownership();
          }

          ~renderbuffer()
          {
            if (!link)
              glDeleteRenderbuffers(1, &id);
          }

          renderbuffer &give_up_ownership()
          {
            link = true;
            return *this;
          }

          renderbuffer &assume_ownership()
          {
            link = false;
            return *this;
          }

          bool is_link() const
          {
            return link;
          }

          GLuint get_id() const
          {
            return id;
          }

          void bind() const
          {
            glBindRenderbuffer(GL_RENDERBUFFER, id);
          }

          void use() const
          {
            glBindRenderbuffer(GL_RENDERBUFFER, id);
          }

          void unbind() const
          {
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
          }

          void unuse() const
          {
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
          }


          void set_storage(ct::vector2 size, GLenum internalformat, size_t samples = 0)
          {
            bind();
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalformat, size.x, size.y);
          }

        private:
          GLuint id;
          bool link;
      };
    } // namespace texture
  } // namespace yaggler
} // namespace neam

#endif /*__N_14995976871411965052_2102803391__OPENGL_RENDERBUFFER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

