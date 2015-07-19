//
// file : obb.hpp
// in : file:///home/tim/projects/yaggler/yaggler/klmb/object/obb.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 28/02/2014 11:12:26
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

#ifndef __N_21004899202013147642_859233986__OBB_HPP__
# define __N_21004899202013147642_859233986__OBB_HPP__

#include <glm_udl.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <klmb/object/aabb.hpp>

namespace neam
{
  namespace klmb
  {
    namespace yaggler
    {
      /// \brief Oriented Bounding Box
      /// Like an aabb, but oriented
      struct obb
      {
        aabb bounding_box; ///< \brief the non-transformed aligned bb (aabb in local space)

        // transformifications (inverted)
        glm::vec3 position = 0_vec3_xyz; ///< \brief inverted position (world -> local)
        glm::vec3 scale = 1_vec3_xyz; ///< \brief inverted scale (world -> local)
        glm::quat rotation = glm::quat(); ///< \brief inverted rotation (world -> local)

        // orig_* are non-inverted transformations
        glm::quat orig_rotation = glm::quat(); ///< \brief rotation (local -> world)
        glm::vec3 orig_scale = 1_vec3_xyz; ///< \brief scale (local -> world)

        obb() {}

        /// \brief construct from an aabb
        obb(const aabb &bbox)
          : bounding_box(bbox)
        {
        }

        /// \brief construct from an obb
        obb(const obb &bbox)
          : bounding_box(bbox.bounding_box), position(bbox.position), scale(bbox.scale), rotation(bbox.rotation), orig_rotation(bbox.orig_rotation), orig_scale(bbox.orig_scale)
        {
        }

        obb &operator = (const aabb &o)
        {
          bounding_box = o;
          return *this;
        }

        obb & operator = (const obb&o)
        {
          bounding_box = o.bounding_box;
          position = o.position;
          scale = o.scale;
          rotation = o.rotation;
          orig_rotation = o.orig_rotation;
          orig_scale = o.orig_scale;

          return *this;
        }

        /// \brief Set the transformations of the obb
        void set_transform(const glm::vec3 &_position, const glm::vec3 &_scale, const glm::quat &_rotation)
        {
          position = _position;
          scale = 1.0f / _scale;
          rotation = glm::conjugate(_rotation);
          orig_rotation = (_rotation);
          orig_scale = _scale;
        }

        /// \brief Set the aabb AND its transformations
        void set_transform_from(const aabb &o, const glm::vec3 &_position, const glm::vec3 &_scale, const glm::quat &_rotation)
        {
          bounding_box = o;
          position = _position;
          scale = 1.0f / _scale;
          rotation = glm::conjugate(_rotation);
          orig_rotation = (_rotation);
          orig_scale = _scale;
        }

        /// \brief Check if a point is inside an OBB
        bool is_inside(const glm::vec3 &pos) const
        {
          return bounding_box.is_inside(untransform_position(pos));
        }

        /// \brief Check if a ray intersects an OBB
        /// \param pos a position alongside the ray
        /// \param dir the ray direction
        /// \param[out] distances the intersection point distances
        bool intersect(const glm::vec3 &pos, const glm::vec3 &dir, glm::vec2 &distances) const
        {
          return bounding_box.intersect(untransform_position(pos), untransform_direction(dir), distances);
        }

        /// \brief add a point (in world coords) to the obb
        void add_world_vertex(const glm::vec3 &point)
        {
          bounding_box.add_vertex(untransform_position(point));
        }

        /// \brief add a point (in local coords) to the obb
        void add_local_vertex(const glm::vec3 &point)
        {
          bounding_box.add_vertex(point);
        }

        /// \brief add an aabb to the obb (in world coords)
        /// \todo make it work
        void add_world_aabb(const aabb &aa)
        {
          // TODO: Fix this as it, obviously enought, isn't correct
          bounding_box.add_vertex(untransform_position(aa.max));
          bounding_box.add_vertex(untransform_position(aa.min));
        }

        /// \brief add an aabb to the obb (in local coords)
        void add_local_aabb(const aabb &aa)
        {
          bounding_box.add_aabb(aa);
        }

        /// \brief add an obb to the obb (in world coords)
        /// \todo make it work
        void add_world_obb(const obb &o)
        {
          // TODO: Fix this as it, obviously enought, isn't correct
          bounding_box.add_vertex(untransform_position(o.transform_direction(o.bounding_box.max)));
          bounding_box.add_vertex(untransform_position(o.transform_direction(o.bounding_box.min)));
        }

        /// \brief add an obb to the obb (in world coords)
        /// \todo make it work
        void add_local_obb(const obb &o)
        {
          // TODO: Fix this as it, obviously enought, isn't correct
          bounding_box.add_vertex(o.transform_direction(o.bounding_box.max));
          bounding_box.add_vertex(o.transform_direction(o.bounding_box.min));
        }

        // utility functions

        /// \brief transform world -> local
        glm::vec3 untransform_position(const glm::vec3 &pos) const
        {
          return  rotation * ((pos - position) * scale);
        }

        /// \brief transform local -> world
        glm::vec3 transform_position(const glm::vec3 &pos) const
        {
          return (orig_rotation * pos) * orig_scale + position;;
        }

        // only apply scale and reverse rotation.
        // no translation

        /// \brief transform world -> local, but for directions only (no translation applied)
        glm::vec3 untransform_direction(const glm::vec3 &dir) const
        {
          return rotation * (dir * scale);
        }

        /// \brief transform local -> world, but for directions only (no translation applied)
        glm::vec3 transform_direction(const glm::vec3 &dir) const
        {
          return (orig_rotation * dir) * orig_scale;
        }

        /// \brief project this obb and return an AABB (the 2D bounding box + depth)
        /// \todo make it work
        aabb project(glm::mat4 *vp_matrix) const
        {
          // apply Rotation Scale Translation (RST)
          auto _rst_proj = [&](const glm::vec3 &pt) -> glm::vec3
          {
            glm::vec3 tmp_rst_vec = (orig_rotation * pt) * orig_scale + position;
            if (!vp_matrix)
              return tmp_rst_vec;
            else
            {
              glm::vec4 tmp_proj_vec = (*vp_matrix) * glm::vec4(tmp_rst_vec, 1.f);
              float w = tmp_proj_vec.w;
              tmp_proj_vec /= static_cast<float>(fabs(tmp_proj_vec.w));
              tmp_proj_vec.x = tmp_proj_vec.x * 0.5f + 0.5f;
              tmp_proj_vec.y = tmp_proj_vec.y * 0.5f + 0.5f;
              return glm::vec3(tmp_proj_vec.x, tmp_proj_vec.y, w);
            }
          };


          aabb tmp;

          // lower plane
          tmp.add_vertex(_rst_proj(glm::vec3(bounding_box.min.x, bounding_box.min.y, bounding_box.min.z)));
          tmp.add_vertex(_rst_proj(glm::vec3(bounding_box.min.x, bounding_box.min.y, bounding_box.max.z)));
          tmp.add_vertex(_rst_proj(glm::vec3(bounding_box.max.x, bounding_box.min.y, bounding_box.min.z)));
          tmp.add_vertex(_rst_proj(glm::vec3(bounding_box.max.x, bounding_box.min.y, bounding_box.max.z)));

          // upper plane
          tmp.add_vertex(_rst_proj(glm::vec3(bounding_box.min.x, bounding_box.max.y, bounding_box.min.z)));
          tmp.add_vertex(_rst_proj(glm::vec3(bounding_box.min.x, bounding_box.max.y, bounding_box.max.z)));
          tmp.add_vertex(_rst_proj(glm::vec3(bounding_box.max.x, bounding_box.max.y, bounding_box.min.z)));
          tmp.add_vertex(_rst_proj(glm::vec3(bounding_box.max.x, bounding_box.max.y, bounding_box.max.z)));

          return tmp;
        }
      };
    } // namespace yaggler
  } // namespace klmb
} // namespace neam

#endif /*__N_21004899202013147642_859233986__OBB_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

