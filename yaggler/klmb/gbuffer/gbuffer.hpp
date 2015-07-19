//
// file : gbuffer.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/gbuffer/gbuffer.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 21/02/2014 20:45:48
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

#ifndef __N_1441800365281942518_451178187__GBUFFER_HPP__
# define __N_1441800365281942518_451178187__GBUFFER_HPP__

#include <tools/ct_string.hpp>
#include <texture/texture.hpp>
#include <klmb/material/material_usings.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      // (notable) addition to yaggler (that induce a different comportment, so,
      // another namespace)
      namespace gbuffer
      {
        // shader stuff, to be included in every material.
        extern constexpr neam::string_t gbuffer_geom_file = "data/shaders/gbuffer/gbuffer.geom";
        extern constexpr neam::string_t gbuffer_frag_file = "data/shaders/gbuffer/gbuffer.frag";

        using gbuffer_geom_shader = auto_file_shader<gbuffer_geom_file>;
        using gbuffer_frag_shader = auto_file_shader<gbuffer_frag_file>;

        /// \brief create and hold a gbuffer with three custom textures and a depthbuffer (and a fbo)
        class gbuffer
        {
          public:
            neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> fbo; ///< \brief the frame buffer object
            neam::yaggler::texture::texture<neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>> color_0; ///< \brief the first color texture
            neam::yaggler::texture::texture<neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>> color_1; ///< \brief the second color texture
            neam::yaggler::texture::texture<neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>> color_2; ///< \brief the third color texture
            neam::yaggler::texture::renderbuffer<neam::yaggler::type::opengl> depthbuffer; ///< \brief the depthbuffer

            GLenum clear_bits = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

          public:
            gbuffer()
            {
            }

            gbuffer(const gbuffer &gb)
              : fbo(gb.fbo), color_0(gb.color_0), color_1(gb.color_1),
                depthbuffer(gb.depthbuffer)
            {
            }

            gbuffer(gbuffer &&gb)
              : fbo(std::move(gb.fbo)), color_0(std::move(gb.color_0)), color_1(std::move(gb.color_1)),
                depthbuffer(std::move(gb.depthbuffer))
            {
            }

            gbuffer(gbuffer &gb, stole_ownership_t)
              : fbo(std::move(gb.fbo)), color_0(std::move(gb.color_0)), color_1(std::move(gb.color_1)),
                depthbuffer(std::move(gb.depthbuffer))
            {
            }

            /// \brief late init/update, setup texture sizes
            /// \note MUST BE CALLED. (at least once)
            void set_screen_size(const ct::vector2 &screen_size)
            {
              color_0.set_texture_sampler(0);
              color_1.set_texture_sampler(1);
              color_2.set_texture_sampler(2);

              color_0.set_gl_parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
              color_0.set_gl_parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
              color_1.set_gl_parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
              color_1.set_gl_parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
              color_2.set_gl_parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
              color_2.set_gl_parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

              color_0.set_texture_data(GL_RGBA, screen_size, GL_RED, GL_BYTE, nullptr, 0);
              color_1.set_texture_data(GL_RGBA32F, screen_size, GL_RED, GL_BYTE, nullptr, 0);
              color_2.set_texture_data(GL_RGBA32F, screen_size, GL_RED, GL_BYTE, nullptr, 0);

              depthbuffer.set_storage(screen_size, GL_DEPTH_COMPONENT24, 0);

              fbo.bind_texture_color(color_0, 0);
              fbo.bind_texture_color(color_1, 1);
              fbo.bind_texture_color(color_2, 2);
              fbo.bind_renderbuffer(depthbuffer, GL_DEPTH_ATTACHMENT);

              GLenum bufs[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
              glDrawBuffers(3, bufs);
            }

            /// \brief bind the fbo for drawing + clear it
            void use()
            {
              fbo.use_draw();
              glClear(clear_bits);
            }
        };

      } // namespace gbuffer
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_1441800365281942518_451178187__GBUFFER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 


