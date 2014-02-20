//
// file : frame_buffer.hpp
// in : file:///home/tim/projects/yaggler/yaggler/texture/frame_buffer.hpp
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


#ifndef __N_65281327319805357_321962912__OPENGL_FRAME_BUFFER_HPP__
# define __N_65281327319805357_321962912__OPENGL_FRAME_BUFFER_HPP__

#include <GLEW/glew.h>
#include <texture/options.hpp>
#include "texture_base.hpp"
#include <tools/ownership.hpp>
#include <yaggler_type.hpp>

namespace neam
{
  namespace yaggler
  {
    namespace texture
    {
      // a basic framebuffer
      template<typename... Args>
      class frame_buffer<type::opengl, Args...>
      {
        public:
          frame_buffer()
            : id(0), link(false)
          {
            glGenFramebuffers(1, &id);
          }

          frame_buffer(GLuint _id)
            : id(_id), link(true)
          {
          }

          frame_buffer(GLuint _id, assume_ownership_t)
            : id(_id), link(false)
          {
          }

          frame_buffer(const frame_buffer &fb)
            : id(fb.id), link(true)
          {
          }

          frame_buffer(frame_buffer &&fb)
            : id(fb.id), link(fb.link)
          {
            fb.link = true;
          }

          frame_buffer(frame_buffer &fb, stole_ownership_t)
            : id(fb.id), link(fb.link)
          {
            fb.link = true;
          }

          ~frame_buffer()
          {
            if (!link)
              glDeleteFramebuffers(1, &id);
          }

          frame_buffer &give_up_ownership()
          {
            link = true;
            return *this;
          }

          frame_buffer &assume_ownership()
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

          // frame buffer related functions

          // draw/read | both
          void use() const
          {
            glBindFramebuffer(GL_FRAMEBUFFER, id);
          }
          void use_read() const
          {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
          }
          void use_draw() const
          {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
          }

          // bind texture to color only
          template<typename TextureType, typename... TArgs>
          void bind_texture_color(const texture<type::opengl, TextureType, TArgs...> &tex, unsigned int color_attachement_index, unsigned int level = 0)
          {
            _sub_bind_texture(GL_FRAMEBUFFER, tex, GL_COLOR_ATTACHMENT0 + color_attachement_index, level);
          }

          template<typename TextureType, typename... TArgs>
          void bind_texture_color(const texture<type::opengl, TextureType, TArgs...> &tex, unsigned int color_attachement_index, unsigned int layer, unsigned int level = 0)
          {
            _sub_bind_texture(GL_FRAMEBUFFER, tex, GL_COLOR_ATTACHMENT0 + color_attachement_index, level, layer);
          }

          template<typename TextureType, typename... TArgs>
          void bind_texture_color(GLenum target, const texture<type::opengl, TextureType, TArgs...> &tex, unsigned int color_attachement_index, unsigned int level = 0)
          {
            _sub_bind_texture(target, tex, GL_COLOR_ATTACHMENT0 + color_attachement_index, level);
          }

          template<typename TextureType, typename... TArgs>
          void bind_texture_color(GLenum target, const texture<type::opengl, TextureType, TArgs...> &tex, unsigned int color_attachement_index, unsigned int layer, unsigned int level = 0)
          {
            _sub_bind_texture(target, tex, GL_COLOR_ATTACHMENT0 + color_attachement_index, level, layer);
          }

          // bind textures, 'generic'
          template<typename TextureType, typename... TArgs>
          void bind_texture(const texture<type::opengl, TextureType, TArgs...> &tex, GLenum attachement, unsigned int level = 0)
          {
            _sub_bind_texture(GL_FRAMEBUFFER, tex, attachement, level);
          }

          template<typename TextureType, typename... TArgs>
          void bind_texture(const texture<type::opengl, TextureType, TArgs...> &tex, GLenum attachement, unsigned int layer, unsigned int level = 0)
          {
            _sub_bind_texture(GL_FRAMEBUFFER, tex, attachement, level, layer);
          }

          template<typename TextureType, typename... TArgs>
          void bind_texture(GLenum target, const texture<type::opengl, TextureType, TArgs...> &tex, GLenum attachement, unsigned int level = 0)
          {
            _sub_bind_texture(target, tex, attachement, level);
          }

          template<typename TextureType, typename... TArgs>
          void bind_texture(GLenum target, const texture<type::opengl, TextureType, TArgs...> &tex, GLenum attachement, unsigned int layer, unsigned int level = 0)
          {
            _sub_bind_texture(target, tex, attachement, level, layer);
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
          bool link;
      };
    } // namespace texture
  } // namespace yaggler
} // namespace neam

#endif /*__N_65281327319805357_321962912__OPENGL_FRAME_BUFFER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on;

