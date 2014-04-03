//
// file : renderable.hpp
// in : file:///home/tim/projects/yaggler/yaggler/bleunw/gui/renderable.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 24/03/2014 10:46:09
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

#ifndef __N_5683124711521397899_592550374__RENDERABLE_HPP__
# define __N_5683124711521397899_592550374__RENDERABLE_HPP__

#include <glm/glm.hpp>

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace gui
      {
        using handle_t = size_t;

        class renderable
        {
          public:
            virtual ~renderable() {}

            virtual void render() = 0;
            virtual void render() const = 0;

          public:
            glm::mat4 *world_pos = nullptr;
            glm::mat4 **vp_matrix = nullptr;

            handle_t handle; // handle in the gui::manager
        };
      } // namespace gui
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_5683124711521397899_592550374__RENDERABLE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

