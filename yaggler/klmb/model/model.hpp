//
// file : model.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/model/model.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 16/02/2014 15:35:20
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

#ifndef __N_1970144924742387235_1074139927__MODEL_HPP__
# define __N_1970144924742387235_1074139927__MODEL_HPP__

#include <glm/glm.hpp>

#include <klmb/object/object.hpp>
#include <klmb/object/obb.hpp>
#include <klmb/object/transformation_tree.hpp>
#include <klmb/camera/camera_holder.hpp>
#include <klmb/material/generic_material.hpp>



namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      namespace transformation_node
      {

        struct default_node;
      }

      /// \brief gather in one place klmb::object + position + material.
      /// This is an instance of a mesh (or a klmb::object)
      class model
      {
        public:
          object<> geometry; ///< \brief the object geometry

          glm::mat4 **vp_matrix = nullptr; ///< \brief a pointer to the vp_matrix of the camera_holder.
          glm::mat4 *world_pos = nullptr;  ///< \brief a pointer to the world_pos of the transformation_tree node.

          material_wrapper material; ///< \brief the material

          transformation_node::default_node *node = nullptr; ///< \brief the node (aka. the position of the model in the world space)

        public:
          /// \brief construct by: linking geometry to _geometry and linking material to _material
          model(const object<> &_geometry, glm::mat4 **_vp_matrix = nullptr, glm::mat4 *_world_pos = nullptr, const material_wrapper &_material = material_wrapper())
            : geometry(_geometry), vp_matrix(_vp_matrix), world_pos(_world_pos), material(_material) {}

          /// \brief construct by: transfering the ownership from _geometry to geometry and linking material to _material
          model(object<> && _geometry, glm::mat4 **_vp_matrix = nullptr, glm::mat4 *_world_pos = nullptr, const material_wrapper & _material = material_wrapper())
            : geometry(std::move(_geometry)), vp_matrix(_vp_matrix), world_pos(_world_pos), material((_material)) {}

          /// \brief construct by: transfering the ownership from _geometry to geometry and from _material to material
          model(object<> &&_geometry, glm::mat4 **_vp_matrix, glm::mat4 *_world_pos, material_wrapper &&_material)
            : geometry(std::move(_geometry)), vp_matrix(_vp_matrix), world_pos(_world_pos), material(std::move(_material)) {}

          /// \brief construct by: linking geometry to _geometry and transfering the ownership from _material to material
          model(const object<> & _geometry, glm::mat4 **_vp_matrix, glm::mat4 *_world_pos, material_wrapper && _material)
            : geometry((_geometry)), vp_matrix(_vp_matrix), world_pos(_world_pos), material(std::move(_material)) {}

          /// \brief draw the model
          /// \todo some optimisations (check for visibility, ...)
          void draw()
          {
            if (!material.is_empty())
            {
              // check for bounding box visibility
              if (node)
              {
                // TODO: fix this.
//                 aabb pbb = node->transformed_bounding_box.project(*vp_matrix);

                // not visible
//                 if ((pbb.max.x < -1.f || pbb.min.x > 1.f) || (pbb.max.y < -1.f || pbb.min.y > 1.f) || pbb.max.z < 0.)
//                   return;
              }
              material.get_object_matrix() = world_pos;
              if (vp_matrix)
                material.get_vp_matrix() = *vp_matrix;
              material.use();
              geometry.draw();
            }
          }

        private:
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_1970144924742387235_1074139927__MODEL_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

