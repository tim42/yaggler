//
// file : pass.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/compositor/pass.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 20/02/2014 20:24:35
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

#ifndef __N_7839127161880462381_1699572152__PASS_HPP__
# define __N_7839127161880462381_1699572152__PASS_HPP__

#include <texture/texture_base.hpp>
#include <texture/opengl_texture.hpp>

#include <klmb/material/generic_material.hpp>
#include <klmb/material/material.hpp>
#include <klmb/object/object.hpp>
#include <klmb/object/default_objects.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      /// \brief a single pass in a compositor
      class compositor_pass
      {
        public:
          /// \brief initialize an emtpy pass
          compositor_pass()
            : fsquad(create_object_from_object_data(object_data::fs_quad).convert_to_generic())
          {
          }

          /// \brief initialize from another compositor pass (copy)
          compositor_pass(const compositor_pass &cp)
            : fsquad(create_object_from_object_data(object_data::fs_quad).convert_to_generic()),
              material(cp.material)
          {
          }

          /// \brief initialize from another compositor pass (move)
          compositor_pass(compositor_pass &&cp)
            : fsquad(std::move(cp.fsquad)),
              material(std::move(cp.material))
          {
          }

          /// \brief initialize from another compositor pass (ownership shift)
          compositor_pass(compositor_pass &cp, stole_ownership_t)
            : fsquad(std::move(cp.fsquad)),
              material(std::move(cp.material))
          {
          }

          /// \brief render the compositor pass
          void render()
          {
            material.use();
            fsquad.draw();
          }

          /// \brief set the fragment shader and its parameters
          /// \param mctp is a list of material_ctx_pair
          template<typename Shader, typename... MaterialCtxPairArgs>
          void set_fragment_shader(MaterialCtxPairArgs... mctp)
          {
            material = neam::klmb::yaggler::create_material_ptr
            <
              // SHADERS
              neam::klmb::yaggler::shader_list
              <
                // we could share this stage across other instance: no changes will be done.
                neam::klmb::yaggler::special_shaders::klmb_fsquad_vert,
                Shader
              >,
              // TEXTURES
              neam::klmb::yaggler::no_textures
            >
            // CONTEXT
            (
              neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative),
              mctp...
            );
            material.assume_ownership();
          }

        private:
          neam::klmb::yaggler::object<> fsquad;
          neam::klmb::yaggler::material_wrapper material;
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_7839127161880462381_1699572152__PASS_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

