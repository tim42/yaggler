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
#include <ct_point.hpp>
#include <yaggler_type.hpp>
#include <GLEW/glew.h>

namespace neam
{
  namespace yaggler
  {
    namespace texture
    {
      /// \brief a renderbuffer class for wrapping GL renderbuffers
      template<>
      class renderbuffer<type::opengl>
      {
        public:
          /// \brief default constructor, create a GL renderbuffer and manage it
          renderbuffer()
            : id(0), ownership(true)
          {
            glGenRenderbuffers(1, &id);
          }

          /// \brief wrap an existing GL renderbuffer, but don't become the owner of the GL renderbuffer
          /// Thus allowing the renderbuffer to be managed for a short time by this class without being destructed at the end of the instance lifetime.
          renderbuffer(GLuint _id)
            : id(_id), ownership(false)
          {
          }

          /// \brief wrap an existing GL renderbuffer and become the owner of the GL renderbuffer
          /// Thus allowing the renderbuffer to be managed by this class and having its lifetime tied to the lifetime of the created instance
          renderbuffer(GLuint _id, assume_ownership_t)
            : id(_id), ownership(true)
          {
          }

          /// \brief wrap an existing renderbuffer, but don't become the owner of the GL renderbuffer
          /// Thus allowing the renderbuffer to be managed for a short time by this class without being destructed at the end of the instance lifetime.
          renderbuffer(const renderbuffer &rb)
            : id(rb.get_id()), ownership(false)
          {
          }

          /// \brief wrap an existing renderbuffer and become the owner of the GL renderbuffer iff \p rb has the ownership
          renderbuffer(renderbuffer &&rb)
            : id(rb.get_id()), ownership(rb.has_ownership())
          {
            rb.give_up_ownership();
          }

          /// \brief wrap an existing renderbuffer and become the owner of the GL renderbuffer iff \p rb has the ownership
          renderbuffer(renderbuffer &rb, stole_ownership_t)
            : id(rb.get_id()), ownership(rb.has_ownership())
          {
            rb.give_up_ownership();
          }

          /// \brief destructor
          ~renderbuffer()
          {
            if (ownership)
              glDeleteRenderbuffers(1, &id);
          }

          /// \brief give up ownership of the GL object,
          /// dissociating the lifetime of the GL object from the lifetime of this instance
          renderbuffer &give_up_ownership()
          {
            ownership = false;
            return *this;
          }

          /// \brief give up ownership of the GL object,
          /// forcing the lifetime of the GL object to be at most the lifetime of this instance
          renderbuffer &assume_ownership()
          {
            ownership = true;
            return *this;
          }

          /// \brief return true if the instance has ownership of the wrapped GL object
          bool has_ownership() const
          {
            return ownership;
          }

          /// \brief wrap an existing renderbuffer and become the owner of the GL renderbuffer iff \p t has the ownership
          renderbuffer &stole(renderbuffer &t)
          {
            if (&t != this)
            {
              if (ownership)
                glDeleteRenderbuffers(1, &id);

              ownership = t.has_ownership();
              id = t.get_id();
              t.give_up_ownership();
            }
            return *this;
          }

          /// \brief wrap an existing renderbuffer, but don't become the owner of the GL renderbuffer
          renderbuffer &link_to(const renderbuffer &t)
          {
            if (&t != this)
            {
              if (ownership)
                glDeleteRenderbuffers(1, &id);

              ownership = false;
              id = t.get_id();
            }
            return *this;
          }

          /// \brief return the ID of the wrapped GL renderbuffer
          GLuint get_id() const
          {
            return id;
          }

          /// \brief bind the renderbuffer
          void bind() const
          {
            glBindRenderbuffer(GL_RENDERBUFFER, id);
          }

          /// \brief bind the renderbuffer
          /// \deprecated for compatibility only
          /// \see bind()
          void use() const
          {
            glBindRenderbuffer(GL_RENDERBUFFER, id);
          }

          /// \brief unbind the renderbuffer
          void unbind() const
          {
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
          }

          /// \brief unbind the renderbuffer
          /// \deprecated for compatibility only
          /// \see unbind()
          void unuse() const
          {
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
          }

          /// \brief set the storage format and size of the renderbuffer
          /// \note after this operation, the buffer will be still bound
          /// \see glRenderbufferStorageMultisample()
          void set_storage(ct::vector2 size, GLenum internalformat, size_t samples = 0)
          {
            bind();
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalformat, size.x, size.y);
          }

        private:
          GLuint id;
          bool ownership;
      };
    } // namespace texture
  } // namespace yaggler
} // namespace neam

#endif /*__N_14995976871411965052_2102803391__OPENGL_RENDERBUFFER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

