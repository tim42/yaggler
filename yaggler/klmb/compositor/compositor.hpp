//
// file : compositor.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/compositor/compositor.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 20/02/2014 19:59:05
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

#ifndef __N_603245356577486585_950303849__COMPOSITOR_HPP__
# define __N_603245356577486585_950303849__COMPOSITOR_HPP__

#include <tools/execute_pack.hpp>

#include <klmb/compositor/pass.hpp>
#include <klmb/compositor/framebuffer_pack.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      template<size_t... Indexes>
      struct input_texture_indexes {};

      template<size_t... Indexes>
      struct output_texture_indexes
      {
        GLenum attachment[sizeof...(Indexes)];
      };

      template<size_t... Indexes, typename... AttachmentType>
      output_texture_indexes<Indexes...> make_output_indexes(AttachmentType... attachment)
      {
        return output_texture_indexes<Indexes...>{{(GLuint)attachment...}};
      }

      /// \brief a multipass, full screen compositor
      /// A "pass" is simply a fragment shader applyed to the whole framebuffer
      /// A pass could have input and output textures and custom uniform bindings
      /// \see compositor_pass for mode detail about how passes are implemented.
      template<typename... Textures>
      class compositor
      {
        public:
          /// \brief initialize the compositor from a framebuffer_pack
          /// \see make_compositor()
          compositor(const framebuffer_pack<Textures...> &_pack)
            : pack(_pack)
          {
          }

          /// \brief initialize the compositor from a temporary framebuffer_pack
          /// \see make_compositor()
          compositor(framebuffer_pack<Textures...> &&_pack)
            : pack(std::move(_pack))
          {
          }

          /// \brief move a compositor (must be a compositor of exactly the same type)
          compositor(compositor &&o)
            : pack(std::move(o.pack)), passes(std::move(o.passes))
          {
          }

          /// \brief copy a compositor (must be a compositor of exactly the same type)
          compositor(const compositor &o)
            : pack(o.pack), passes(o.passes)
          {
          }

          /// \brief add an output pass (a pass that will output to the framebuffer in the framebuffer_pack)
          /// \param gl_clear is forwarded to glClear (except if it's 0 which means "do not clear")
          /// \param input_texture_indexes<InputTextureIndex...> the list of textures to be bound to specific uniforms
          /// \param amctp a list of additional material_ctx_pair to describe how uniforms will be bounded
          template<typename Shader, size_t... InputTextureIndex, typename... AdditionalMaterialCtxPairArgs>
          compositor &add_pass(GLenum gl_clear, input_texture_indexes<InputTextureIndex...>, AdditionalMaterialCtxPairArgs... amctp)
          {
            compositor_pass pass;

            pass.set_fragment_shader<Shader>
            (
              neam::klmb::yaggler::make_ctx_pair(pack.textures.template get<InputTextureIndex>().uniform_name, pack.textures.template get<InputTextureIndex>().sampler)...,
              amctp...
            );

            passes.emplace_back(pass_holder<InputTextureIndex...>(std::move(pass), pack.framebuffer, gl_clear));

            return *this;
          }

          /// \brief standard pass (with an output framebuffer created for it)
          /// \param gl_clear is forwarded to glClear (except if it's 0 which means "do not clear")
          /// \param oidxs (for Output InDeXeS) the list of textures to be used as output for this pass
          /// \param input_texture_indexes<InputTextureIndex...> the list of textures to be bound to their uniforms
          /// \param amctp a list of additional material_ctx_pair to describe how uniforms will be bounded
          template<typename Shader, size_t... InputTextureIndex, size_t... OutputTextureIndex, typename... AdditionalMaterialCtxPairArgs>
          compositor &add_pass(GLenum gl_clear, const output_texture_indexes<OutputTextureIndex...> &oidxs, input_texture_indexes<InputTextureIndex...>,  AdditionalMaterialCtxPairArgs... amctp)
          {
            compositor_pass pass;

            pass.set_fragment_shader<Shader>
            (
              neam::klmb::yaggler::make_ctx_pair(pack.textures.template get<InputTextureIndex>().uniform_name, pack.textures.template get<InputTextureIndex>().sampler)...,
              amctp...
            );

            neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> tmp_framebuffer;

            size_t i = 0;
            NEAM_EXECUTE_PACK((tmp_framebuffer.bind_texture(pack.textures.template get<OutputTextureIndex>().texture, oidxs.attachment[i++])));

            passes.emplace_back(pass_holder<InputTextureIndex...>(std::move(pass), std::move(tmp_framebuffer), gl_clear));

            return *this;
          }

          /// \brief render the compositors and all its passes
          void render()
          {
            for (auto &it : passes)
            {
              it.render(*this);
            }
          }

          /// \brief the pack used
          /// \see framebuffer_pack
          framebuffer_pack<Textures...> pack;

        private:
          template<size_t... TIdxs>
          struct pass_holder
          {
            compositor_pass pass;
            neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> framebuffer;
            GLenum clear;

            pass_holder(const compositor_pass &_pass, const neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> &_fb, GLenum _clear) : pass(_pass), framebuffer(_fb), clear(_clear) {}
            pass_holder(compositor_pass && _pass, neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> && _fb, GLenum _clear) : pass(std::move(_pass)), framebuffer(std::move(_fb)), clear(_clear) {}
            pass_holder(const compositor_pass & _pass, neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> && _fb, GLenum _clear) : pass((_pass)), framebuffer(std::move(_fb)), clear(_clear) {}
            pass_holder(compositor_pass && _pass, const neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl> & _fb, GLenum _clear) : pass(std::move(_pass)), framebuffer((_fb)), clear(_clear) {}

            pass_holder(const pass_holder &_pass) : pass(_pass.pass), framebuffer(_pass.framebuffer), clear(_pass.clear) {}
            pass_holder(pass_holder && _pass) : pass(std::move(_pass.pass)), framebuffer(std::move(_pass.framebuffer)), clear(_pass.clear) {}

            void use_textures(const compositor &c) const
            {
              c.pack.template use_textures<TIdxs...>();
            }

            void render(const compositor &c)
            {
              framebuffer.use_draw();

              if (clear)
                glClear(clear);

              use_textures(c);
              pass.render();
            }
          };

          class gen_pass_holder
          {
            public:
              template<typename PassHolder>
              gen_pass_holder(const PassHolder &passh)
                : hldr(new spec_gen_pass_holder<PassHolder>(passh))
              {
              }

              template<typename PassHolder>
              gen_pass_holder(PassHolder &&passh)
                : hldr(new spec_gen_pass_holder<PassHolder>(std::move(passh)))
              {
              }

              gen_pass_holder(gen_pass_holder &&gpassh)
                : hldr(gpassh.hldr)
              {
                gpassh.hldr = nullptr;
              }

              ~gen_pass_holder()
              {
                if (hldr)
                  delete hldr;
              }

              void render(const compositor &c)
              {
                if (hldr)
                  hldr->render(c);
              }

            private: // I do like nested classes.
              class base_gen_pass_holder
              {
                public:
                  virtual ~base_gen_pass_holder() {}
                  virtual void render(const compositor &c) = 0;
              };
              template<typename PassHolder>
              class spec_gen_pass_holder : public base_gen_pass_holder
              {
                public:
                  spec_gen_pass_holder(const PassHolder &hld) : passh(hld) {}
                  spec_gen_pass_holder(PassHolder &&hld) : passh(std::move(hld)) {}

                  virtual ~spec_gen_pass_holder()
                  {
                  }

                  virtual void render(const compositor &c) final
                  {
                    passh.render(c);
                  }

                  PassHolder passh;
              };

            private:
              base_gen_pass_holder *hldr;
          };

        private:
          std::deque<gen_pass_holder> passes;
      };

      /// \brief create a compositor by specifying the framebuffer pack to use.
      /// \param Textures is inferred from the framebuffer_pack type
      /// \note You should use make_framebuffer_pack() to handle the creation of a framebuffer pack
      template<typename... Textures>
      compositor<Textures...> make_compositor(const framebuffer_pack<Textures...> &_pack)
      {
        return compositor<Textures...>(_pack);
      }

      /// \brief create a compositor by specifying the framebuffer pack to use.
      /// \param Textures is inferred from the framebuffer_pack type
      /// \note You should use make_framebuffer_pack() to handle the creation of a framebuffer pack
      template<typename... Textures>
      compositor<Textures...> make_compositor(framebuffer_pack<Textures...> &&_pack)
      {
        return compositor<Textures...>(std::move(_pack));
      }

    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_603245356577486585_950303849__COMPOSITOR_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 


