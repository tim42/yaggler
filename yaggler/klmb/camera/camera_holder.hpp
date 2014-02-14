//
// file : camera_holder.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/camera/camera_holder.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 13/02/2014 18:13:51
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

#ifndef __N_21431251471359755128_1526022208__CAMERA_HOLDER_HPP__
# define __N_21431251471359755128_1526022208__CAMERA_HOLDER_HPP__

#include <klmb/camera/camera.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      // a 'placeholder' camera man,
      // allow mainly to switch camera on the fly
      struct camera_holder
      {
        const glm::mat4 *vp_matrix = nullptr; // yep, that's it.

        template<typename CamType>
        void use_camera(const CamType &cam)
        {
          vp_matrix = &(cam.vp_matrix);
        }
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_21431251471359755128_1526022208__CAMERA_HOLDER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

