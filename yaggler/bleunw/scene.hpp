//
// file : scene.hpp
// in : file:///home/tim/projects/yaggler/yaggler/bleunw/scene.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 20/02/2014 10:21:45
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

#ifndef __N_663677697293323614_2069131433__SCENE_HPP__
# define __N_663677697293323614_2069131433__SCENE_HPP__

#include <klmb/camera/camera.hpp>
#include <klmb/camera/camera_holder.hpp>

#include <klmb/object/transformation_tree.hpp>
#include <klmb/object/object.hpp>

#include <klmb/model/model.hpp>

#include <set>
#include <deque>

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace scene
      {
        template<typename TrTreeNodeType = neam::klmb::yaggler::transformation_node::default_node>
        class manager
        {
          public: // types
            template<typename Type>
            using container = std::set<Type>;

            template<typename Type>
            using indexed_container = std::deque<Type>;

          public: // constructors
            virtual ~manager()
            {
            }

            manager() {}

            // only the 'data' part is copied (object + materials)
            manager(const manager &m)
              : transformation_tree(), objects(m.objects), materials(m.materials)
            {
            }

            manager(manager &m, stole_ownership_t)
              : transformation_tree(), objects(std::move(m.objects)), materials(std::move(m.materials))
            {
            }

            manager(manager &&m)
              : transformation_tree(), objects(std::move(m.objects)), materials(std::move(m.materials))
            {
            }

            template<typename OTrTreeNodeType>
            manager(const manager<OTrTreeNodeType> &m)
              : transformation_tree(), objects(m.objects), materials(m.materials)
            {
            }

            template<typename OTrTreeNodeType>
            manager(manager<OTrTreeNodeType> &m, stole_ownership_t)
              : transformation_tree(), objects(std::move(m.objects)), materials(std::move(m.materials))
            {
            }

            template<typename OTrTreeNodeType>
            manager(manager<OTrTreeNodeType> &&m)
              : transformation_tree(), objects(std::move(m.objects)), materials(std::move(m.materials))
            {
            }

          public:
            klmb::yaggler::transformation_tree<TrTreeNodeType> transformation_tree; ///< \brief The transformation tree (hold the relative / absolute transformation for each models / cameras)

            // data
            indexed_container<klmb::yaggler::object<>> objects; ///< \brief The list of objects (the geometry buffers)
            indexed_container<klmb::yaggler::material_wrapper> materials; ///< \brief The list of materials

            // render
            klmb::yaggler::camera_holder camera_holder; ///< \brief The current camera
            indexed_container<klmb::yaggler::camera *> camera_list; ///< \brief The list of perspective cameras
            indexed_container<klmb::yaggler::ortho_camera *> ortho_camera_list; ///< \brief The list of orthographic cameras

            indexed_container<klmb::yaggler::model *> model_list; ///< \brief The list of models

          public: // operations
            /// \brief Update the scene (the camera and all of the dirtied matrices)
            void update()
            {
              transformation_tree.root.recompute_matrices();
              camera_holder.recompute_cam_matrices();
            }

            /// \brief render the scene
            void render()
            {
              for (klmb::yaggler::model *model : model_list)
                model->draw();
            }

            /// \brief render the scene, const version
            void render() const
            {
              for (klmb::yaggler::model *model : model_list)
                model->draw();
            }
        };
      } // namespace scene
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_663677697293323614_2069131433__SCENE_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

