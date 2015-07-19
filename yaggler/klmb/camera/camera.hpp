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
      /// \brief a simple camera (basically a struct that holds a glm::mat4)
      struct camera
      {
        glm::mat4 vp_matrix; ///< \brief the result (to be passed as ref<> to shaders / objects /...)

        glm::mat4 *world_matrix = nullptr; /// \brief link this to any node_holder world matrix.

        // some controls vars
        float fov = M_PI / 4.;
        float aspect = 4. / 3.;

        float near = 0.1;
        float far = 10000.;

        // camera specifics
        glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 up_vector = glm::vec3(0.0, 1.0, 0.0);

        // it's either target_lock or look_at (if target_lock is null)
        glm::vec3 *target_lock = nullptr; ///< \brief if not null, a pointer to a vec3 holding the target position
        glm::vec3 look_at = glm::vec3(0.0, 0.0, 1.0); ///< \brief if target_lock is not null, a position to look at

        // intermediate steps
        glm::mat4 view_matrix;
        glm::mat4 proj_matrix;

        /// \brief recompute all matrices
        void recompute_matrices()
        {
          _recompute_proj_matrix();
          _recompute_view_matrix();
          _recompute_vp_matrix();

        }
        /// \brief recompute projection and viewprojection matrices (but not the view one)
        void recompute_proj_and_vp_matrices()
        {
          _recompute_proj_matrix();
          _recompute_vp_matrix();
        }
        /// \brief recompute view and viewprojection matrices (but not the projection one)
        void recompute_view_and_vp_matrices()
        {
          _recompute_view_matrix();
          _recompute_vp_matrix();
        }

        /// \brief recompute only the vp matrix
        /// \note advanced
        void _recompute_vp_matrix()
        {
          vp_matrix = proj_matrix * view_matrix;
        }

        /// \brief recompute only the view matrix
        /// \note advanced (vp is not recomputed)
        void _recompute_view_matrix()
        {
          glm::vec3 look = look_at;
          if (target_lock)
            look = *target_lock;

          if (world_matrix)
          {
            glm::vec4 tpos = ((*world_matrix) * glm::vec4(position, 1.));
            view_matrix = glm::lookAt(glm::vec3(tpos.x, tpos.y, tpos.z), look, up_vector);
          }
          else
            view_matrix = glm::lookAt(position, look, up_vector);
        }

        /// \brief recompute only the projection matrix
        /// \note advanced (vp is not recomputed)
        void _recompute_proj_matrix()
        {
          if (isinf(far))
            proj_matrix = glm::infinitePerspective(fov, aspect, near);
          else
            proj_matrix = glm::perspective(fov, aspect, near, far);
        }

        /// \brief constructor.
        camera()
        {
          recompute_matrices();
        }
      };

      /// \brief an orthographic camera
      struct ortho_camera
      {
        glm::mat4 vp_matrix; ///< \brief the result (to be passed as ref<> to shaders / objects /...)

        // some controls vars
        glm::vec2 min = glm::vec2(-1, -1);
        glm::vec2 max = glm::vec2(1, 1);

        float near = 0;
        float far = 100;

        /// \brief recompute matrix
        void recompute_matrices()
        {
          vp_matrix = glm::ortho(min.x, max.x, min.y, max.y, near, far);
        }

        /// \brief recompute matrix
        void recompute_proj_and_vp_matrices()
        {
          recompute_matrices();
        }

        /// \brief recompute matrix
        void recompute_view_and_vp_matrices()
        {
          recompute_matrices();
        }

        /// \brief constructor.
        ortho_camera()
        {
          recompute_matrices();
        }
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_1685533877553007173_1165765898__CAMERA_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

