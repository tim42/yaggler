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

#include <deque>
#include <tools/execute_pack.hpp>
#include <texture/texture.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      /// \brief for usage in framebuffer_packs, create a reference to a texture, its binding uniform and sampler indexes.
      /// \see make_texture_entry()
      template<typename Texture>
      struct texture_entry
      {
        /// \brief initialize a texture entry
        /// \param _uniform_name the name of the uniform the bound the texture
        /// \param _texture the texture to bind
        /// \param _sampler the sampler index to use
        texture_entry(const std::string &_uniform_name, const Texture &_texture, size_t _sampler) : uniform_name(_uniform_name), texture(_texture), sampler(_sampler) {}

        /// \brief initialize a texture entry
        /// \param _uniform_name the name of the uniform the bound the texture
        /// \param _texture the texture to bind
        /// \param _sampler the sampler index to use
        texture_entry(const std::string &_uniform_name, Texture &&_texture, size_t _sampler) : uniform_name(_uniform_name), texture(std::move(_texture)), sampler(_sampler) {}

        std::string uniform_name;
        Texture texture;
        GLint sampler;
      };

      /// \brief an helper for creating texture_entry instances (by deducing the template argument).
      /// \param name the name of the uniform the bound the texture
      /// \param texture the texture to bind
      /// \param sampler the sampler index to use
      template<typename Texture>
      texture_entry<Texture> make_texture_entry(const std::string &name, const Texture &texture, size_t sampler)
      {
        return texture_entry<Texture>(name, texture, sampler);
      }

      /// \brief an helper for creating texture_entry instances (by deducing the template argument).
      /// \param name the name of the uniform the bound the texture
      /// \param texture the texture to bind
      /// \param sampler the sampler index to use
      template<typename Texture>
      texture_entry<Texture> make_texture_entry(const std::string &name, typename std::remove_reference<Texture>::type &&texture, size_t sampler)
      {
        return texture_entry<Texture>(name, std::move((texture)), sampler);
      }

      /// \brief a framebuffer + textures pack-class
      /// \note textures aren't attached to the framebuffer, it's just a class that hold both for compositors.
      template<typename... Textures>
      struct framebuffer_pack
      {
        public:
          cr::tuple<texture_entry<Textures>...> textures; ///< \brief the texture list
          neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> framebuffer; ///< \brief the framebuffer to use as output (for output passes only)

          /// \brief initialize a framebuffer pack
          /// \see make_framebuffer_pack()
          framebuffer_pack(const neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> &fmb, texture_entry<Textures>... _textures)
            : textures(_textures...), framebuffer(fmb)
          {
            set_textures_samplers(cr::gen_seq<sizeof...(Textures)>());
          }

          /// \brief initialize a framebuffer pack
          /// \see make_framebuffer_pack()
          framebuffer_pack(neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> && fmb, texture_entry<Textures>... _textures)
            : textures(_textures...), framebuffer(std::move(fmb))
          {
            set_textures_samplers(cr::gen_seq<sizeof...(Textures)>());
          }

          /// \brief initialize a framebuffer pack from another one
          framebuffer_pack(const framebuffer_pack &fb)
            : textures(fb.textures), framebuffer(fb.framebuffer)
          {
          }

          /// \brief initialize a framebuffer pack from another one
          framebuffer_pack(framebuffer_pack && fb)
            : textures(std::move(fb.textures)), framebuffer(std::move(fb.framebuffer))
          {
          }

          /// \brief initialize a framebuffer pack from another one
          framebuffer_pack(framebuffer_pack &fb, stole_ownership_t)
            : textures(std::move(fb.textures)), framebuffer(std::move(fb.framebuffer))
          {
          }

          /// \brief bind the framebuffer of the pack
          void use() const
          {
            framebuffer.use();
          }

          /// \brief some of the textures of the pack
          template<size_t... Idxs>
          void use_textures() const
          {
            NEAM_EXECUTE_PACK((textures.template get<Idxs>().texture.use()));
          }

          /// \brief bind all the textures of the pack
          void use_textures() const
          {
            use_textures(cr::gen_seq<sizeof...(Textures)>());
          }

          /// \brief bind a list of texture following a given integer sequence
          template<size_t... Idxs>
          void use_textures(cr::seq<Idxs...>) const
          {
            use_textures<Idxs...>();
          }

        private:
          template<size_t... Idxs>
          void set_textures_samplers(cr::seq<Idxs...>)
          {
            NEAM_EXECUTE_PACK((textures.template get<Idxs>().texture.set_texture_sampler(textures.template get<Idxs>().sampler)));
          }
      };

      /// \brief create a framebuffer pack (deduce the Texture... template parameter)
      /// \param fmb is the framebuffer to use
      /// \param _textures is a list of texture_entry to pack with the framebuffer
      template<typename... Textures>
      framebuffer_pack<Textures...> make_framebuffer_pack(const neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> &fmb, texture_entry<Textures>... _textures)
      {
        return framebuffer_pack<Textures...>(fmb, _textures...);
      }

      /// \brief create a framebuffer pack (deduce the Texture... template parameter)
      /// \param fmb is the framebuffer to use
      /// \param _textures is a list of texture_entry to pack with the framebuffer
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

