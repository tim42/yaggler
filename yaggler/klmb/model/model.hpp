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
#include <klmb/camera/camera_holder.hpp>
#include <klmb/material/generic_material.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      // object + position + material.
      class model
      {
        public:
//           camera_holder *holder;
          glm::mat4 **vp_matrix = nullptr; // a pointer to the vp_matrix of the camera_holder.
          glm::mat4 *world_pos = nullptr;  // a pointer to the world_pos of the transformation_tree node.


          // the object geometry
          object<> geometry;

          // the material
          material_wrapper material;

          void draw()
          {
            material.get_object_matrix() = world_pos;
            if (vp_matrix)
              material.get_vp_matrix() = *vp_matrix;
            material.use();
            geometry.draw();
          }

        private:
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_1970144924742387235_1074139927__MODEL_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

