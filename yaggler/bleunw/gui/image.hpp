//
// file : image.hpp
// in : file:///home/tim/projects/yaggler/yaggler/bleunw/gui/image.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 15/04/2014 09:04:43
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

#ifndef __N_727405482749488537_1330091139__IMAGE_HPP__
# define __N_727405482749488537_1330091139__IMAGE_HPP__

#include <klmb/material/material.hpp>
#include <klmb/material/generic_material.hpp>

#include <klmb/object/object.hpp>
#include <klmb/object/default_objects.hpp>
#include "renderable.hpp"

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace gui
      {
        namespace defaults
        {
          constexpr neam::string_t image_fragment_shader_file = "data/shaders/bleunw/image/image.frag";
          constexpr neam::string_t image_9slices_fragment_shader_file = "data/shaders/bleunw/image/9slices.frag";

          using image_fragment_shader = klmb::yaggler::auto_file_shader<image_fragment_shader_file>;
          using image_9slices_fragment_shader = klmb::yaggler::auto_file_shader<image_9slices_fragment_shader_file>;
        } // namespace defaults

        // this is ~an image, as it takes a framgent shader and some infos to build the material (a bit like framebuffer_pack)
        // this is a simple case of a material with a plane associated to it.
        template<typename FragmentShader>
        class base_image : public renderable
        {
          public:
            base_image() : plane(neam::klmb::yaggler::create_object_from_object_data(neam::klmb::yaggler::object_data::positive_quad))
            {
            }

            virtual ~base_image() {}

            template<typename TextureList, typename... AdditionalMaterialCtxPairArgs>
            void set_material(AdditionalMaterialCtxPairArgs... amcp)
            {
              textures.clear();
              textures.reserve(TextureList::size);

              auto *mptr = neam::klmb::yaggler::create_material_ptr
              <
                // SHADERS
                neam::klmb::yaggler::shader_list
                <
                  // we could share this stage across other instance: no changes will be done.
//                   neam::klmb::yaggler::klmb_default_vert,
                  neam::klmb::yaggler::klmb_default_vert,
                  FragmentShader
                >,
                // TEXTURES
                TextureList
              >
              // CONTEXT
              (
                neam::klmb::yaggler::make_ctx_pair("alpha_factor", cr::make_const_ref(alpha_factor)),
                neam::klmb::yaggler::make_ctx_pair("start_uv", cr::make_const_ref(start_uv)),
                neam::klmb::yaggler::make_ctx_pair("end_uv", cr::make_const_ref(end_uv)),
                amcp...
              );
              fill_textures(mptr, cr::gen_seq<TextureList::size>());

              material = mptr;
              material.assume_ownership();
              material.get_object_matrix() = world_pos;
              material.get_vp_matrix() = *vp_matrix;
            }

            // the image filename is automatically loaded and bound to the 'image' uniform.
            template<typename... AdditionalMaterialCtxPairArgs>
            void set_material(const std::string &image_filename, AdditionalMaterialCtxPairArgs... amcp)
            {
              textures.clear();
              textures.reserve(1);

              // load that texture
              {
                neam::yaggler::texture::options::png_texture_loader<GL_RGBA8> loader(image_filename);
                neam::yaggler::texture::texture<neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>> texture;
                texture.set_texture_data(loader);
                textures.emplace_back(texture, neam::stole_ownership);
              }

              auto *mptr = neam::klmb::yaggler::create_material_ptr
              <
                // SHADERS
                neam::klmb::yaggler::shader_list
                <
                  // we could share this stage across other instance: no changes will be done.
//                   neam::klmb::yaggler::klmb_default_vert,
                  neam::klmb::yaggler::klmb_default_vert,
                  FragmentShader
                >,
                // TEXTURES
                neam::klmb::yaggler::no_textures
              >
              // CONTEXT
              (
                neam::klmb::yaggler::make_ctx_pair("image", cr::make_const_ref(textures[0])),
                neam::klmb::yaggler::make_ctx_pair("alpha_factor", cr::make_const_ref(alpha_factor)),
                neam::klmb::yaggler::make_ctx_pair("start_uv", cr::make_const_ref(start_uv)),
                neam::klmb::yaggler::make_ctx_pair("end_uv", cr::make_const_ref(end_uv)),
                amcp...
              );

              material = mptr;
              material.assume_ownership();
              material.get_object_matrix() = world_pos;
              material.get_vp_matrix() = *vp_matrix;
            }

            template<typename... TextureParams, typename... AdditionalMaterialCtxPairArgs>
            void set_material(const neam::yaggler::texture::texture< neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>, TextureParams...> &txt, AdditionalMaterialCtxPairArgs... amcp)
            {
              textures.clear();
              textures.reserve(1);

              // keep that texture in memory (keep only a link to it).
              textures.emplace_back(txt);

              auto *mptr = neam::klmb::yaggler::create_material_ptr
              <
                // SHADERS
                neam::klmb::yaggler::shader_list
                <
                  // we could share this stage across other instance: no changes will be done.
//                   neam::klmb::yaggler::klmb_default_vert,
                  neam::klmb::yaggler::klmb_default_vert,
                  FragmentShader
                >,
                // TEXTURES
                neam::klmb::yaggler::no_textures
              >
              // CONTEXT
              (
                neam::klmb::yaggler::make_ctx_pair("image", cr::make_const_ref(textures[0])),
                neam::klmb::yaggler::make_ctx_pair("alpha_factor", cr::make_const_ref(alpha_factor)),
                neam::klmb::yaggler::make_ctx_pair("start_uv", cr::make_const_ref(start_uv)),
                neam::klmb::yaggler::make_ctx_pair("end_uv", cr::make_const_ref(end_uv)),
                amcp...
              );

              material = mptr;
              material.assume_ownership();
              material.get_object_matrix() = world_pos;
              material.get_vp_matrix() = *vp_matrix;
            }

            template<GLenum TType, typename... AdditionalMaterialCtxPairArgs>
            void set_material(const neam::yaggler::texture::options::png_texture_loader<TType> &ld, AdditionalMaterialCtxPairArgs... amcp)
            {
              textures.clear();
              textures.reserve(1);

              // keep that texture in memory.
              {
                neam::yaggler::texture::texture<neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_2D>> texture;
                texture.set_texture_data(ld);
                textures.emplace_back(texture, neam::stole_ownership);
              }

              auto *mptr = neam::klmb::yaggler::create_material_ptr
              <
                // SHADERS
                neam::klmb::yaggler::shader_list
                <
                  // we could share this stage across other instance: no changes will be done.
//                   neam::klmb::yaggler::klmb_default_vert,
                  neam::klmb::yaggler::klmb_default_vert,
                  FragmentShader
                >,
                // TEXTURES
                neam::klmb::yaggler::no_textures
              >
              // CONTEXT
              (
                neam::klmb::yaggler::make_ctx_pair("image", cr::make_const_ref(textures[0])),
                neam::klmb::yaggler::make_ctx_pair("alpha_factor", cr::make_const_ref(alpha_factor)),
                neam::klmb::yaggler::make_ctx_pair("start_uv", cr::make_const_ref(start_uv)),
                neam::klmb::yaggler::make_ctx_pair("end_uv", cr::make_const_ref(end_uv)),
                amcp...
              );

              material = mptr;
              material.assume_ownership();
              material.get_object_matrix() = world_pos;
              material.get_vp_matrix() = *vp_matrix;
            }

            template<typename... AdditionalMaterialCtxPairArgs>
            void set_material(AdditionalMaterialCtxPairArgs... amcp)
            {
              textures.clear();

              auto *mptr = neam::klmb::yaggler::create_material_ptr
              <
                // SHADERS
                neam::klmb::yaggler::shader_list
                <
                  // we could share this stage across other instance: no changes will be done.
//                   neam::klmb::yaggler::klmb_default_vert,
                  neam::klmb::yaggler::klmb_default_vert,
                  FragmentShader
                >,
                // TEXTURES
                neam::klmb::yaggler::no_textures
              >
              // CONTEXT
              (
                neam::klmb::yaggler::make_ctx_pair("alpha_factor", cr::make_const_ref(alpha_factor)),
                neam::klmb::yaggler::make_ctx_pair("start_uv", cr::make_const_ref(start_uv)),
                neam::klmb::yaggler::make_ctx_pair("end_uv", cr::make_const_ref(end_uv)),
                amcp...
              );

              material = mptr;
              material.assume_ownership();
              material.get_object_matrix() = world_pos;
              material.get_vp_matrix() = *vp_matrix;
            }

            virtual void render() const
            {
              // we have something to render
              if (!material.is_empty())
              {
                for (auto &it : textures)
                  it.use();
                material.use();
                plane.draw();
              }
            }

            virtual void render()
            {
              static_cast<const base_image *>(this)->render();
            }

          public:
            float alpha_factor = 1.;
            glm::vec2 start_uv = glm::vec2(0, 0);
            glm::vec2 end_uv = glm::vec2(1, 1);

            std::vector<neam::yaggler::texture::texture< neam::yaggler::type::opengl, embed::GLenum<GL_TEXTURE_2D> >> textures;

          private:
            template<typename MaterialType, size_t... Idxs>
            void fill_textures(MaterialType *mptr, cr::seq<Idxs...>)
            {
              NEAM_EXECUTE_PACK(textures.push_back(mptr->template get_texture<Idxs>()));
            }

          private:
            neam::klmb::yaggler::object<GL_ARRAY_BUFFER> plane;
            neam::klmb::yaggler::material_wrapper material;

        };

        using image = base_image<defaults::image_fragment_shader>;
        using image_9slices = base_image<defaults::image_9slices_fragment_shader>;

      } // namespace gui
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_727405482749488537_1330091139__IMAGE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

