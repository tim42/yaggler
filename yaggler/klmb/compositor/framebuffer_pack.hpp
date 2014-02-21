//
// file : framebuffer_pack.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/compositor/framebuffer_pack.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 20/02/2014 20:48:43
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

#ifndef __N_539314345523947050_1106712426__FRAMEBUFFER_PACK_HPP__
# define __N_539314345523947050_1106712426__FRAMEBUFFER_PACK_HPP__

#include <texture/texture.hpp>
#include <deque>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      template<typename Texture>
      struct texture_entry
      {
        texture_entry(const std::string &_uniform_name, const Texture &_texture, size_t _sampler) : uniform_name(_uniform_name), texture(_texture), sampler(_sampler) {}
        texture_entry(const std::string &_uniform_name, Texture &&_texture, size_t _sampler) : uniform_name(_uniform_name), texture(std::move(_texture)), sampler(_sampler) {}

        std::string uniform_name;
        Texture texture;
        GLint sampler;
      };

      template<typename Texture>
      texture_entry<Texture> make_texture_entry(const std::string &name, const Texture &texture, size_t sampler)
      {
        return texture_entry<Texture>(name, texture, sampler);
      }
//       template<typename Texture>
//       texture_entry<Texture> make_texture_entry(const std::string &name, Texture &&texture, size_t sampler)
//       {
//         return texture_entry<Texture>(name, std::move(texture), sampler);
//       }

      // a framebuffer + textures pack-class
      // NOTE: textures aren't attached to the framebuffer, it's just a class that hold both for compositors.
      template<typename... Textures>
      struct framebuffer_pack
      {
        cr::tuple<texture_entry<Textures>...> textures;
        neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> framebuffer;

        framebuffer_pack(const neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> &fmb, texture_entry<Textures>... _textures)
          : textures(_textures...), framebuffer(fmb)
        {
          // TODO: init Texture samplers
//           void((int []){(_textures.texture.set_texture_sampler(_textures.sampler), 5)...});
//           void((int []){(framebuffer.bind_texture(_textures.texture, _textures.attachement), 5)...});
        }
        framebuffer_pack(neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> &&fmb, texture_entry<Textures>... _textures)
          : textures(_textures...), framebuffer(std::move(fmb))
        {
          // TODO: init Texture samplers
//           void((int []){(_textures.texture.set_texture_sampler(_textures.sampler), 5)...});
//           void((int []){(framebuffer.bind_texture(_textures.texture, _textures.attachement), 5)...});
        }

        framebuffer_pack(const framebuffer_pack &fb)
          : textures(fb.textures), framebuffer(fb.framebuffer)
        {
        }

        framebuffer_pack(framebuffer_pack &&fb)
          : textures(std::move(fb.textures)), framebuffer(std::move(fb.framebuffer))
        {
        }

        framebuffer_pack(framebuffer_pack &fb, stole_ownership_t)
          : textures(std::move(fb.textures)), framebuffer(std::move(fb.framebuffer))
        {
        }

        // construct the output framebuffer
        framebuffer_pack(int)
          : textures(), framebuffer(0)
        {
        }

        void use() const
        {
          framebuffer.use();
        }

        template<size_t... Idxs>
        void use_textures() const
        {
          void((int []){(textures.template get<Idxs>().texture.use(), 5)...});
        }

        void use_textures() const
        {
          use_textures(cr::gen_seq<sizeof...(Textures)>());
        }

        template<size_t... Idxs>
        void use_textures(cr::seq<Idxs...>) const
        {
          use_textures<Idxs...>();
        }
      };

      template<typename... Textures>
      framebuffer_pack<Textures...> make_framebuffer_pack(const neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> &fmb, texture_entry<Textures>... _textures)
      {
        return framebuffer_pack<Textures...>(fmb, _textures...);
      }
      template<typename... Textures>
      framebuffer_pack<Textures...> make_framebuffer_pack(neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> &&fmb, texture_entry<Textures>... _textures)
      {
        return framebuffer_pack<Textures...>(std::move(fmb), _textures...);
      }

    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_539314345523947050_1106712426__FRAMEBUFFER_PACK_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

