//
// file : camera.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/camera/camera.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 13/02/2014 17:47:11
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

#ifndef __N_1685533877553007173_1165765898__CAMERA_HPP__
# define __N_1685533877553007173_1165765898__CAMERA_HPP__

#include <cmath>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      // a simple camera (basically a struct that holds a glm::mat4)
      struct camera
      {
        glm::mat4 vp_matrix; // the result (to be passed as ref<> to shaders / objects /...)

        // some controls vars
        float fov = M_PI / 4.;
        float aspect = 4. / 3.;

        float near = 0.1;
        float far = 1000.;

        // camera specifics
        glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 up_vector = glm::vec3(0.0, 1.0, 0.0);
        glm::vec3 look_at = glm::vec3(0.0, 0.0, 1.0);

        // intermediate steps
        glm::mat4 view_matrix;
        glm::mat4 proj_matrix;

        // recompute matrices
        void recompute_matrices()
        {
          _recompute_proj_matrix();
          _recompute_view_matrix();
          _recompute_vp_matrix();
        }
        void recompute_proj_and_vp_matrices()
        {
          _recompute_proj_matrix();
          _recompute_vp_matrix();
        }
        void recompute_view_and_vp_matrices()
        {
          _recompute_view_matrix();
          _recompute_vp_matrix();
        }

        // recompute single matrices (marked as advanced because vp_matrix is not recomputed)
        void _recompute_vp_matrix()
        {
          vp_matrix = proj_matrix * view_matrix;
        }
        void _recompute_view_matrix()
        {
          view_matrix = glm::lookAt(position, look_at, up_vector);
        }
        void _recompute_proj_matrix()
        {
          if (isinf(far))
            proj_matrix = glm::infinitePerspective(fov, aspect, near);
          else
            proj_matrix = glm::perspective(fov, aspect, near, far);
        }

        // constructor.
        camera()
        {
          recompute_matrices();
        }
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_1685533877553007173_1165765898__CAMERA_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

