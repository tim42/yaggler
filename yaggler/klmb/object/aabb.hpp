//
// file : aabb.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/object/aabb.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 22/02/2014 21:42:10
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

#ifndef __N_178120621236830485_1940062634__AABB_HPP__
# define __N_178120621236830485_1940062634__AABB_HPP__

#include <glm_udl.hpp>

#include <glm/glm.hpp>
#include <algorithm>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      struct aabb
      {
        glm::vec3 min = glm::vec3(INFINITY, INFINITY, INFINITY);
        glm::vec3 max = glm::vec3(-INFINITY, -INFINITY, -INFINITY);

        aabb() {}
        aabb(const aabb &o) : min(o.min), max(o.max) {}
        aabb(const glm::vec3 &_min, const glm::vec3 &_max) : min(_min), max(_max) {}

        void add_aabb(const aabb &o)
        {
          add_vertex(o.min);
          add_vertex(o.max);
        }

        void add_vertex(const glm::vec3 &point)
        {
          if (point.x < min.x)
            min.x = point.x;
          else if (point.x > max.x)
            max.x = point.x;

          if (point.y < min.y)
            min.y = point.y;
          else if (point.y > max.y)
            max.y = point.y;

          if (point.z < min.z)
            min.z = point.z;
          else if (point.z > max.z)
            max.z = point.z;
        }

        bool is_inside(const glm::vec3 &point) const
        {
          return (point.x >= min.x && point.x <= max.x && point.y >= min.y && point.z <= max.z);
        }

        bool intersect(const glm::vec3 &pos, const glm::vec3 &dir, glm::vec2 &distances) const
        {
          // from http://prideout.net/blog/p64/SinglePass.glsl
          // AND  http://gamedev.stackexchange.com/a/18459
          glm::vec3 invR = 1.0f / glm::normalize(dir);
          glm::vec3 tbot = invR * (min - pos);
          glm::vec3 ttop = invR * (max - pos);
          glm::vec3 tmin = glm::min(ttop, tbot);
          glm::vec3 tmax = glm::max(ttop, tbot);
          distances.x = glm::max(glm::max(glm::min(tmin.x, tmax.x), glm::min(tmin.y, tmax.y)), glm::min(tmin.z, tmax.z));
          distances.y = glm::min(glm::min(glm::max(tmin.x, tmax.x), glm::max(tmin.y, tmax.y)), glm::max(tmin.z, tmax.z));
          return distances.x <= distances.y && distances.y >= 0;
        }
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_178120621236830485_1940062634__AABB_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

