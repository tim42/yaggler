//
// file : framebuffer.hpp
// in : file:///home/tim/projects/yaggler/yaggler/texture/framebuffer.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 14/01/2014 16:24:12
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


#ifndef __N_65281327319805357_321962912__OPENGL_FRAMEBUFFER_HPP__
# define __N_65281327319805357_321962912__OPENGL_FRAMEBUFFER_HPP__

#include <GLEW/glew.h>
#include <texture/options.hpp>
#include "texture_base.hpp"
#include "opengl_renderbuffer.hpp"
#include <tools/ownership.hpp>
#include <yaggler_type.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace texture
    {
      /// \brief a framebuffer class for wrapping GL framebuffers
      template<>
      class framebuffer<type::opengl>
      {
        public:
          /// \brief default constructor, create a GL framebuffer and manage it
          framebuffer()
            : id(0), ownership(true)
          {
            glGenFramebuffers(1, &id);
          }

          /// \brief wrap an existing GL framebuffer, but don't become the owner of the GL framebuffer
          /// Thus allowing the framebuffer to be managed for a short time by this class without being destructed at the end of the instance lifetime.
          framebuffer(GLuint _id)
            : id(_id), ownership(false)
          {
          }

          /// \brief wrap an existing GL framebuffer and become the owner of the GL framebuffer
          /// Thus allowing the framebuffer to be managed by this class and having its lifetime tied to the lifetime of the created instance
          framebuffer(GLuint _id, assume_ownership_t)
            : id(_id), ownership(true)
          {
          }

          /// \brief wrap an existing framebuffer, but don't become the owner of the GL framebuffer
          /// Thus allowing the framebuffer to be managed for a short time by this class without being destructed at the end of the instance lifetime.
          framebuffer(const framebuffer &fb)
            : id(fb.id), ownership(false)
          {
          }

          /// \brief wrap an existing framebuffer and become the owner of the GL framebuffer iff \p fb has the ownership
          framebuffer(framebuffer &&fb)
            : id(fb.id), ownership(fb.ownership)
          {
            fb.ownership = false;
          }

          /// \brief wrap an existing framebuffer and become the owner of the GL framebuffer iff \p fb has the ownership
          framebuffer(framebuffer &fb, stole_ownership_t)
            : id(fb.id), ownership(fb.ownership)
          {
            fb.ownership = false;
          }

          /// \brief destructor
          ~framebuffer()
          {
            if (ownership)
              glDeleteFramebuffers(1, &id);
          }

          /// \brief give up ownership of the GL object,
          /// dissociating the lifetime of the GL object from the lifetime of this instance
          framebuffer &give_up_ownership()
          {
            ownership = false;
            return *this;
          }

          /// \brief give up ownership of the GL object,
          /// forcing the lifetime of the GL object to be at most the lifetime of this instance
          framebuffer &assume_ownership()
          {
            ownership = true;
            return *this;
          }

          /// \brief return true if the instance has ownership of the wrapped GL object
          bool has_ownership() const
          {
            return ownership;
          }

          /// \brief wrap an existing framebuffer and become the owner of the GL framebuffer iff \p t has the ownership
          framebuffer &stole(framebuffer &t)
          {
            if (&t != this)
            {
              if (ownership)
                glDeleteFramebuffers(1, &id);

              ownership = t.has_ownership();
              id = t.get_id();
              t.give_up_ownership();
            }
            return *this;
          }

          /// \brief wrap an existing framebuffer, but don't become the owner of the GL framebuffer
          framebuffer &link_to(const framebuffer &t)
          {
            if (&t != this)
            {
              if (ownership)
                glDeleteFramebuffers(1, &id);

              ownership = false;
              id = t.get_id();
            }
            return *this;
          }

          /// \brief return the ID of the wrapped GL framebuffer
          GLuint get_id() const
          {
            return id;
          }

          // frame buffer related functions

          /// \brief bind for reading and drawing
          void bind() const
          {
            glBindFramebuffer(GL_FRAMEBUFFER, id);
          }

          /// \brief unbind both reading and drawing
          void unbind() const
          {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
          }

          /// \brief bind for reading and drawing
          void use() const
          {
            glBindFramebuffer(GL_FRAMEBUFFER, id);
          }

          /// \brief unbind both reading and drawing
          void unuse() const
          {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
          }

          /// \brief bind for reading only
          void use_read() const
          {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
          }

          /// \brief bind for drawing only
          void use_draw() const
          {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
          }

          /// \brief unbind reading only
          void unuse_read() const
          {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
          }

          /// \brief unbind drawing only
          void unuse_draw() const
          {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
          }

          /// \brief bind a texture to color only
          /// \param tex the texture to bind
          /// \param color_attachement_index the index in the framebuffer to bind the texture
          /// \param level the mipmap level to bind
          template<typename TextureType, typename... TArgs>
          void bind_texture_color(const texture<type::opengl, TextureType, TArgs...> &tex, unsigned int color_attachement_index, unsigned int level = 0)
          {
            use();
            _sub_bind_texture(GL_FRAMEBUFFER, tex, GL_COLOR_ATTACHMENT0 + color_attachement_index, level);
          }

          /// \brief bind a texture (a _ARRAY or a 3D) to color only
          /// \param tex the texture to bind
          /// \param color_attachement_index the index in the framebuffer to bind the texture
          /// \param level the mipmap level to bind
          template<typename TextureType, typename... TArgs>
          void bind_texture_color(const texture<type::opengl, TextureType, TArgs...> &tex, unsigned int color_attachement_index, unsigned int layer, unsigned int level = 0)
          {
            use();
            _sub_bind_texture(GL_FRAMEBUFFER, tex, GL_COLOR_ATTACHMENT0 + color_attachement_index, level, layer);
          }

          /// \brief bind a texture (a _ARRAY or a 3D/2D), color only, to a custom target
          /// \param tex the texture to bind
          /// \param color_attachement_index the index in the framebuffer to bind the texture
          /// \param level the mipmap level to bind
          template<typename TextureType, typename... TArgs>
          void bind_texture_color(GLenum target, const texture<type::opengl, TextureType, TArgs...> &tex, unsigned int color_attachement_index, unsigned int level = 0)
          {
            use();
            _sub_bind_texture(target, tex, GL_COLOR_ATTACHMENT0 + color_attachement_index, level);
          }

          /// \brief bind a texture (a _ARRAY or a 3D/2D), color only, to a custom target
          /// \param tex the texture to bind
          /// \param color_attachement_index the index in the framebuffer to bind the texture
          /// \param layer the layer to bind
          /// \param level the mipmap level to bind
          template<typename TextureType, typename... TArgs>
          void bind_texture_color(GLenum target, const texture<type::opengl, TextureType, TArgs...> &tex, unsigned int color_attachement_index, unsigned int layer, unsigned int level = 0)
          {
            use();
            _sub_bind_texture(target, tex, GL_COLOR_ATTACHMENT0 + color_attachement_index, level, layer);
          }

          /// \brief bind a texture to a custom attachment (depth, color, ...)
          /// \param tex the texture to bind
          /// \param color_attachement_index the index in the framebuffer to bind the texture
          /// \param level the mipmap level to bind
          template<typename TextureType, typename... TArgs>
          void bind_texture(const texture<type::opengl, TextureType, TArgs...> &tex, GLenum attachement, unsigned int level = 0)
          {
            use();
            _sub_bind_texture(GL_FRAMEBUFFER, tex, attachement, level);
          }

          /// \brief bind a texture (a _ARRAY or a 3D/2D) to a custom attachment (depth, color, ...)
          /// \param tex the texture to bind
          /// \param color_attachement_index the index in the framebuffer to bind the texture
          /// \param layer the layer to bind
          /// \param level the mipmap level to bind
          template<typename TextureType, typename... TArgs>
          void bind_texture(const texture<type::opengl, TextureType, TArgs...> &tex, GLenum attachement, unsigned int layer, unsigned int level = 0)
          {
            use();
            _sub_bind_texture(GL_FRAMEBUFFER, tex, attachement, level, layer);
          }

          /// \brief bind a texture (a _ARRAY or a 3D/2D) to a custom target + a custom attachment (depth, color, ...)
          /// \param tex the texture to bind
          /// \param color_attachement_index the index in the framebuffer to bind the texture
          /// \param level the mipmap level to bind
          template<typename TextureType, typename... TArgs>
          void bind_texture(GLenum target, const texture<type::opengl, TextureType, TArgs...> &tex, GLenum attachement, unsigned int level = 0)
          {
            use();
            _sub_bind_texture(target, tex, attachement, level);
          }

          /// \brief bind a texture (a _ARRAY or a 3D/2D) to a custom target + a custom attachment (depth, color, ...)
          /// \param tex the texture to bind
          /// \param color_attachement_index the index in the framebuffer to bind the texture
          /// \param layer the layer to bind
          /// \param level the mipmap level to bind
          template<typename TextureType, typename... TArgs>
          void bind_texture(GLenum target, const texture<type::opengl, TextureType, TArgs...> &tex, GLenum attachement, unsigned int layer, unsigned int level = 0)
          {
            use();
            _sub_bind_texture(target, tex, attachement, level, layer);
          }

          /// \brief bind a renderbuffer to a custom attachment
          /// \param rb the renderbuffer to bind
          /// \param attachment the attachement where the renderbuffer will be bound
          void bind_renderbuffer(const renderbuffer<type::opengl> &rdb, GLenum attachment)
          {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rdb.get_id());
          }

          /// \brief bind a renderbuffer to a custom attachment and a custom target
          /// \param target the target to bind the renderbuffer (default is GL_FRAMEBUFFER)
          /// \param rb the renderbuffer to bind
          /// \param attachment the attachement where the renderbuffer will be bound
          void bind_renderbuffer(GLenum target, const renderbuffer<type::opengl> &rdb, GLenum attachment)
          {
            glFramebufferRenderbuffer(target, attachment, GL_RENDERBUFFER, rdb.get_id());
          }

        private:
          template<typename TextureType, typename... TArgs>
          void _sub_bind_texture(GLenum target, const texture<type::opengl, TextureType, TArgs...> &tex, NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_1D), GLenum) attachement, int level)
          {
            glFramebufferTexture1D(target, attachement, GL_TEXTURE_1D, tex.get_id(), level);
          }
          template<typename TextureType, typename... TArgs>
          void _sub_bind_texture(GLenum target, const texture<type::opengl, TextureType, TArgs...> &tex, NCR_ENABLE_IF((TextureType::value != GL_TEXTURE_1D && TextureType::value != GL_TEXTURE_2D_ARRAY && TextureType::value != GL_TEXTURE_3D && TextureType::value != GL_TEXTURE_2D_MULTISAMPLE_ARRAY), GLenum) attachement, int level)
          {
            glFramebufferTexture2D(target, attachement, TextureType::value, tex.get_id(), level);
          }
          template<typename TextureType, typename... TArgs>
          void _sub_bind_texture(GLenum target, const texture<type::opengl, TextureType, TArgs...> &tex, NCR_ENABLE_IF((TextureType::value == GL_TEXTURE_2D_ARRAY && TextureType::value == GL_TEXTURE_3D && TextureType::value == GL_TEXTURE_2D_MULTISAMPLE_ARRAY), GLenum) attachement, int level, int layer)
          {
            glFramebufferTextureLayer(target, attachement, tex.get_id(), level, layer);
          }

        private:
          GLuint id;
          bool ownership;
      };
    } // namespace texture
  } // namespace yaggler
} // namespace neam

#endif /*__N_65281327319805357_321962912__OPENGL_FRAMEBUFFER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on;

