//
// file : app.hpp
// in : file:///home/tim/projects/yaggler/samples/test-klmb/app.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 20/02/2014 09:45:52
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

#ifndef __N_1657502776544798074_976431267__APP_HPP__
# define __N_1657502776544798074_976431267__APP_HPP__

#include <bleunw/application.hpp>

#include <klmb/klmb.hpp>
#include "loader.hpp"

constexpr neam::string_t vert = "data/shaders/dragon/dragon.vert";
constexpr neam::string_t frag = "data/shaders/dragon/dragon.frag";
constexpr neam::string_t geom = "data/shaders/dragon/dragon.geom";

constexpr neam::string_t yaggler_white_logo = "data/textures/yaggler-w.png";

namespace neam
{
  namespace klmb
  {
    namespace sample
    {
      class main_application : public neam::bleunw::yaggler::application::base_application<main_application>
      {
        public:
          using default_transformation_tree_node_type = neam::klmb::yaggler::transformation_node::default_node;

        public:
          virtual ~main_application() {}

          main_application()
            : base_application(neam::yaggler::glfw_window(neam::yaggler::window_mode::fullscreen))
          {
            init();
          }

          void run()
          {
            cr::chrono chronos;

            while (!window.should_close())
            {
              double delta = chronos.delta();

              // move the object
              object_2_parent_local_node->rotation = glm::rotate(object_2_parent_local_node->rotation, (float)(M_PI / 15. * delta), glm::vec3(0, 1, 0));
              object_2_parent_local_node->dirty = 1;

              // move the cam
              parent_camera_local_node->rotation = glm::rotate(parent_camera_local_node->rotation, (float)(M_PI / -15. * delta), glm::vec3(0, 1, 0));
              parent_camera_local_node->dirty = 1;

              end_render_loop();
            }
          }

        private:
          void init()
          {
            main_smgr.camera_list.insert(&camera);
            main_smgr.camera_holder.use_camera(camera);

            auto &parent_node = main_smgr.transformation_tree.root.create_child();
            auto &object_node = parent_node.create_child();

            object_node.local->position = glm::vec3(0, -3, -0.);
            object_node.local->scale = glm::vec3(25.);
            object_node.local->dirty = true;


            auto &object_2_parent_node = parent_node.create_child();
            object_2_parent_local_node = object_2_parent_node.local;

            auto &object_2_parent_sub_node = object_2_parent_node.create_child();
            auto &object_2_node = object_2_parent_sub_node.create_child();

            object_2_parent_sub_node.local->position = glm::vec3(-5., -0, -0.);
            object_2_parent_sub_node.local->dirty = true;
            object_2_node.local->position = glm::vec3(0., -3., -0.);
            object_2_node.local->scale = glm::vec3(20.);
            object_2_node.local->dirty = true;

            // lock the camera to the object #2.
            camera.target_lock = &(object_2_parent_sub_node.world->position);

            // camera stuff.
            auto &parent_camera_node = main_smgr.transformation_tree.root.create_child();
            parent_camera_local_node = parent_camera_node.local;
            auto &camera_node = parent_camera_node.create_child();

            camera_node.local->position = glm::vec3(0., 0., -10.);

            parent_camera_node.local->dirty = true;
            camera_node.local->dirty = true;

            // make the link.
            camera.world_matrix = &camera_node.world->matrix;


            // material stuff
            neam::klmb::yaggler::material_wrapper material = neam::klmb::yaggler::create_material_ptr
            <
              // SHADERS
              neam::klmb::yaggler::shader_list
              <
                neam::klmb::yaggler::auto_file_shader<vert>,
                neam::klmb::yaggler::auto_file_shader<geom>,
                neam::klmb::yaggler::auto_file_shader<frag>
              >,
              // TEXTURES
              neam::klmb::yaggler::rgba_png_2d_texture_list
              <
                yaggler_white_logo
              >
            >
            // CONTEXT
            (
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
             neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative),
             neam::klmb::yaggler::make_ctx_pair("texture", neam::klmb::yaggler::reference_to_texture<0>())
            );

            main_smgr.materials.push_back(std::move(material));
            material = main_smgr.materials.back(); // 'material' is now a link

            // models
            main_smgr.objects.push_back(neam::klmb::sample::load_model("./data/models/dragon_vrip_res3.ply").convert_to_generic());

            main_smgr.model_list.insert(new neam::klmb::yaggler::model(main_smgr.objects.back(), &main_smgr.camera_holder.vp_matrix, &object_node.world->matrix, material));
            main_smgr.model_list.insert(new neam::klmb::yaggler::model(main_smgr.objects.back(), &main_smgr.camera_holder.vp_matrix, &object_2_node.world->matrix, material));

            // yay: gl calls :D
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
          }

        private:
          neam::klmb::yaggler::camera camera;

          neam::klmb::yaggler::transformation_node::default_node *object_2_parent_local_node;
          neam::klmb::yaggler::transformation_node::default_node *parent_camera_local_node;
      };
    } // namespace sample
  } // namespace klmb
} // namespace neam

#endif /*__N_1657502776544798074_976431267__APP_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

