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

#include "io.hpp"
#include <bleunw/application.hpp>

#include <klmb/klmb.hpp>
#include "loader.hpp"
#include "node.hpp"


constexpr neam::string_t dragon_vert = "data/shaders/dragon/dragon.vert";
constexpr neam::string_t dragon_frag = "data/shaders/dragon/dragon.frag";
constexpr neam::string_t dragon_geom = "data/shaders/dragon/dragon.geom";

constexpr neam::string_t mat3_frag = "data/shaders/editor/mat3.frag";
constexpr neam::string_t mat4_frag = "data/shaders/editor/mat4.frag";
constexpr neam::string_t mat5_frag = "data/shaders/editor/mat5.frag";
constexpr neam::string_t mat6_frag = "data/shaders/editor/mat6.frag";
constexpr neam::string_t mat6_vert = "data/shaders/editor/mat6.vert";
constexpr neam::string_t mat7_frag = "data/shaders/editor/mat7.frag";
constexpr neam::string_t mat8_frag = "data/shaders/editor/mat8.frag";
constexpr neam::string_t mat9_frag = "data/shaders/editor/mat9.frag";

constexpr neam::string_t selected_vert = "data/shaders/editor/sel.vert";
constexpr neam::string_t selected_frag = "data/shaders/editor/sel.frag";
constexpr neam::string_t selected_geom = "data/shaders/editor/sel.geom";


constexpr neam::string_t ssao_compositor_frag = "data/shaders/gbuffer/ssao.frag";
constexpr neam::string_t dof_compositor_frag = "data/shaders/gbuffer/dof.frag";
constexpr neam::string_t flash_compositor_frag = "data/shaders/gbuffer/flash.frag";
constexpr neam::string_t ground_compositor_frag = "data/shaders/gbuffer/ground.frag";

constexpr neam::string_t yaggler_white_logo = "data/textures/yaggler-w.png";

namespace neam
{
  namespace klmb
  {
    namespace sample
    {
      class main_application : public neam::bleunw::yaggler::application::base_application<main_application>,
        neam::bleunw::yaggler::events::keyboard_listener, neam::bleunw::yaggler::events::mouse_listener
      {
        public:
          using default_transformation_tree_node_type = neam::klmb::yaggler::transformation_node::default_node;

        public:
          virtual ~main_application() {}

          main_application()
            : base_application(neam::yaggler::glfw_window(neam::yaggler::window_mode::fullscreen))
//             : base_application(neam::yaggler::glfw_window(neam::yaggler::window_mode::windowed, {1000, 1000}))
          {
            init();
            emgr.register_keyboard_listener(this);
            emgr.register_mouse_listener(this);
          }

          void run()
          {
            cr::chrono chronos;

            glm::mat4 cam_view;
            glm::vec4 cam_pos;

            /// compositors are here: because of their weird types (and no wrapper at this time), it is easier to create them just here.

            // 'temporary gbuffer'
            klmb::yaggler::gbuffer::gbuffer tmp_gbuffer;
            tmp_gbuffer.set_screen_size({(ct::fixed_t)framebuffer_resolution.x, (ct::fixed_t)framebuffer_resolution.y});


            // SCENE GROUND (~raymarched)
            auto scene_ground_compositor = neam::klmb::yaggler::make_compositor(neam::klmb::yaggler::make_framebuffer_pack
            (
              tmp_gbuffer.fbo, // output to tmp_gbuffer

              // gbuffer textures
              neam::klmb::yaggler::make_texture_entry("scene", gbuffer.color_0, 0),
              neam::klmb::yaggler::make_texture_entry("geometry", gbuffer.color_1, 1),

              // temporary
              neam::klmb::yaggler::make_texture_entry("scene", tmp_gbuffer.color_0, 2),
              neam::klmb::yaggler::make_texture_entry("geometry", tmp_gbuffer.color_1, 3)
            ));

            //  [kind of] output pass
            scene_ground_compositor.add_pass<neam::klmb::yaggler::auto_file_shader<ground_compositor_frag>>(0,
                neam::klmb::yaggler::input_texture_indexes<0, 1>(),

                neam::klmb::yaggler::make_ctx_pair("cam_view", neam::cr::make_const_ref(cam_view)),
                neam::klmb::yaggler::make_ctx_pair("cam_pos", neam::cr::make_const_ref(cam_pos)),
                neam::klmb::yaggler::make_ctx_pair("buffer_size", framebuffer_resolution)
            );

            // SSAO
            auto ssao_compositor = neam::klmb::yaggler::make_compositor(neam::klmb::yaggler::make_framebuffer_pack
            (
              neam::yaggler::texture::framebuffer <neam::yaggler::type::opengl> (0), // output to none

              // gbuffer
              neam::klmb::yaggler::make_texture_entry("scene", gbuffer.color_0, 0),
              neam::klmb::yaggler::make_texture_entry("geometry", tmp_gbuffer.color_1, 1),

              // temporary
              neam::klmb::yaggler::make_texture_entry("scene", tmp_gbuffer.color_0, 2)
            ));

            //  output pass
            ssao_compositor.add_pass<neam::klmb::yaggler::auto_file_shader<ssao_compositor_frag>>(GL_COLOR_BUFFER_BIT,
                neam::klmb::yaggler::input_texture_indexes<2, 1>(),
                neam::klmb::yaggler::make_output_indexes<0>(GL_COLOR_ATTACHMENT0),

                neam::klmb::yaggler::make_ctx_pair("buffer_size", framebuffer_resolution)
            );

            // DOF
            auto dof_compositor = neam::klmb::yaggler::make_compositor(neam::klmb::yaggler::make_framebuffer_pack
            (
              neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl>(0), // output to none

              // gbuffer
              neam::klmb::yaggler::make_texture_entry("scene", gbuffer.color_0, 0),
              neam::klmb::yaggler::make_texture_entry("geometry", tmp_gbuffer.color_1, 1),

              // temporary
              neam::klmb::yaggler::make_texture_entry("scene", tmp_gbuffer.color_0, 0)
            ));

            // passes:
            //  non-output pass
            dof_compositor.add_pass<neam::klmb::yaggler::auto_file_shader<dof_compositor_frag>>(GL_COLOR_BUFFER_BIT,
                neam::klmb::yaggler::input_texture_indexes<0, 1>(),
                neam::klmb::yaggler::make_output_indexes<2>(GL_COLOR_ATTACHMENT0),

                neam::klmb::yaggler::make_ctx_pair("direction", glm::vec2(1, -1)),
                neam::klmb::yaggler::make_ctx_pair("max_distance", neam::cr::make_const_ref(blur_max_distance)),
                neam::klmb::yaggler::make_ctx_pair("center", neam::cr::make_const_ref(blur_center)),
                neam::klmb::yaggler::make_ctx_pair("buffer_size", framebuffer_resolution)
            );
            //  output pass
            dof_compositor.add_pass<neam::klmb::yaggler::auto_file_shader<dof_compositor_frag>>(GL_COLOR_BUFFER_BIT,
                neam::klmb::yaggler::input_texture_indexes<2, 1>(),
                neam::klmb::yaggler::make_output_indexes<0>(GL_COLOR_ATTACHMENT0),

                neam::klmb::yaggler::make_ctx_pair("direction", glm::vec2(-1, -1)),
                neam::klmb::yaggler::make_ctx_pair("max_distance", neam::cr::make_const_ref(blur_max_distance)),
                neam::klmb::yaggler::make_ctx_pair("center", neam::cr::make_const_ref(blur_center)),
                neam::klmb::yaggler::make_ctx_pair("buffer_size", framebuffer_resolution)
            );

            // FLASH
            auto flash_compositor = neam::klmb::yaggler::make_compositor(neam::klmb::yaggler::make_framebuffer_pack
            (
              neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl>(0), // output to 0 (screen)

              // gbuffer
              neam::klmb::yaggler::make_texture_entry("scene", gbuffer.color_0/*gbuffer.color_0*/, 0),
              neam::klmb::yaggler::make_texture_entry("geometry", tmp_gbuffer.color_1, 1)
            ));
            //  output pass
            flash_compositor.add_pass<neam::klmb::yaggler::auto_file_shader<flash_compositor_frag>>(0,
                neam::klmb::yaggler::input_texture_indexes<0, 1>(),

                neam::klmb::yaggler::make_ctx_pair("flash_time", neam::cr::make_const_ref(flash_time)),
                neam::klmb::yaggler::make_ctx_pair("flash_color", neam::cr::make_const_ref(flash_color)),
                neam::klmb::yaggler::make_ctx_pair("buffer_size", framebuffer_resolution)
            );

            end_render_loop();

            while (!window.should_close() && !do_quit)
            {
              double delta = chronos.delta();

              glViewport(0, 0, framebuffer_resolution.x, framebuffer_resolution.y);


              tmp_gbuffer.use();
              gbuffer.use();

              // for DoF compo'
//               blur_center = fabs(camera_local_node->position.z);

              // for ground compo'
              cam_view = (elements_smgr.camera_holder._get_std_cam()->view_matrix);
              cam_pos = (*elements_smgr.camera_holder._get_std_cam()->world_matrix) * glm::vec4(elements_smgr.camera_holder._get_std_cam()->position, 1.);

              // move cam
              {
                glm::vec3 tmpadd(camera_movement.x * delta, 0, -camera_movement.y * delta);
                tmpadd = parent_camera_local_node->rotation * tmpadd;
                parent_camera_local_node->position += tmpadd;
                camera.look_at += tmpadd;
                parent_camera_local_node->dirty = 1;
              }

              elements_smgr.render();
              main_smgr.render();

              // this one is a bit strange ;) (raymarched/raytraced compositor)
              scene_ground_compositor.render();

              // render ssao, dof and flash.
              ssao_compositor.render();
              dof_compositor.render();
              flash_compositor.render();

              end_render_loop();
            }
          }

        private:
          void init()
          {
            main_smgr.camera_list.push_back(&camera);

            // single camera, two scenes.
            main_smgr.camera_holder.use_camera(camera);
            elements_smgr.camera_holder.use_camera(camera);

            // lock the camera to the object #2.
//             camera.target_lock = &(object_2_parent_sub_node.world->position);

            // camera stuff.
            auto &parent_camera_node = main_smgr.transformation_tree.root.create_child();
            parent_camera_local_node = parent_camera_node.local;
            auto &camera_node = parent_camera_node.create_child();
            camera_local_node = camera_node.local;

            camera_node.local->position = glm::vec3(0., 7., -20.);

            parent_camera_node.local->dirty = true;
            camera_node.local->dirty = true;

            // make the link.
            camera.world_matrix = &camera_node.world->matrix;


            // material stuff
            load_materials();

            // models
            load_models();

            // yay: gl calls :D
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            // gbuffer
            gbuffer.set_screen_size({(ct::fixed_t)framebuffer_resolution.x, (ct::fixed_t)framebuffer_resolution.y});

            // load the scene (with that green / red flash feedback)
            if (!load("./data/save.scene"))
              flash_color = glm::vec4(1.0, 0.3, 0.0, 1);
            else
              flash_color = glm::vec4(0.5, 1.0, 0.3, 1);
            flash_time = cr::chrono::now_relative();
          }

          virtual void button_pressed(const bleunw::yaggler::events::mouse_status &ms, bleunw::yaggler::events::mouse_buttons::mouse_buttons mb);
          virtual void button_released(const bleunw::yaggler::events::mouse_status &ms, bleunw::yaggler::events::mouse_buttons::mouse_buttons mb);
          virtual void mouse_moved(const bleunw::yaggler::events::mouse_status &ms);

          virtual void key_pressed(const bleunw::yaggler::events::keyboard_status &ks, bleunw::yaggler::events::key_code::key_code kc);
          virtual void key_released(const bleunw::yaggler::events::keyboard_status &ks, bleunw::yaggler::events::key_code::key_code kc);

        private: // load & save
          // save the tree (walk from the root, no recursion, hyper simple format)
          void save(const std::string &filename) const;

          // load the scene from file.
          bool load(const std::string &filename);

          // here again, we use an ineficient parser.
          void load_models()
          {
            // scene models
            elements_smgr.objects.emplace_back(neam::klmb::sample::load_model("./data/models/dragon_vrip_res4.ply").convert_to_generic());
            elements_smgr.objects.emplace_back(neam::klmb::sample::load_model("./data/models/dragon_vrip_res3.ply").convert_to_generic());
            elements_smgr.objects.emplace_back(neam::klmb::sample::load_model("./data/models/happy_vrip_res4.ply").convert_to_generic());
            elements_smgr.objects.emplace_back(neam::klmb::sample::load_model("./data/models/happy_vrip_res3.ply").convert_to_generic());
            elements_smgr.objects.emplace_back(neam::klmb::sample::load_model("./data/models/cube.ply").convert_to_generic());
            elements_smgr.objects.emplace_back(neam::klmb::sample::load_model("./data/models/dragon_vrip.ply").convert_to_generic());
            elements_smgr.objects.emplace_back(neam::klmb::sample::load_model("./data/models/happy_vrip.ply").convert_to_generic());

            // editor models
            main_smgr.objects.push_back(elements_smgr.objects[4]); // [4] is the cube
          }

          // that horrible material stuff here.
          void load_materials()
          {
            elements_smgr.materials.emplace_back(neam::klmb::yaggler::create_material_ptr
            <
              // SHADERS
              neam::klmb::yaggler::shader_list
              <
                neam::klmb::yaggler::auto_file_shader<dragon_vert>,
                neam::klmb::yaggler::gbuffer::gbuffer_geom_shader,
                neam::klmb::yaggler::gbuffer::gbuffer_frag_shader,
                neam::klmb::yaggler::auto_file_shader<dragon_frag>
              >,
              // TEXTURES
              neam::klmb::yaggler::no_textures
            >
            // CONTEXT
            (
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
             neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative)
            ));

            elements_smgr.materials.emplace_back(neam::klmb::yaggler::create_material_ptr
            <
              // SHADERS
              neam::klmb::yaggler::shader_list
              <
                neam::klmb::yaggler::auto_file_shader<dragon_vert>,
                neam::klmb::yaggler::auto_file_shader<dragon_geom>,
                neam::klmb::yaggler::gbuffer::gbuffer_geom_shader,
                neam::klmb::yaggler::gbuffer::gbuffer_frag_shader,
                neam::klmb::yaggler::auto_file_shader<dragon_frag>
              >,
              // TEXTURES
              neam::klmb::yaggler::no_textures
            >
            // CONTEXT
            (
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
              neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative)
            ));

            elements_smgr.materials.emplace_back(neam::klmb::yaggler::create_material_ptr
            <
              // SHADERS
              neam::klmb::yaggler::shader_list
              <
                neam::klmb::yaggler::auto_file_shader<dragon_vert>,
                neam::klmb::yaggler::gbuffer::gbuffer_geom_shader,
                neam::klmb::yaggler::gbuffer::gbuffer_frag_shader,
                neam::klmb::yaggler::auto_file_shader<mat3_frag>
              >,
              // TEXTURES
              neam::klmb::yaggler::no_textures
            >
            // CONTEXT
            (
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
             neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative)
            ));
            elements_smgr.materials.emplace_back(neam::klmb::yaggler::create_material_ptr
            <
              // SHADERS
              neam::klmb::yaggler::shader_list
              <
                neam::klmb::yaggler::auto_file_shader<dragon_vert>,
                neam::klmb::yaggler::gbuffer::gbuffer_geom_shader,
                neam::klmb::yaggler::gbuffer::gbuffer_frag_shader,
                neam::klmb::yaggler::auto_file_shader<mat4_frag>
              >,
              // TEXTURES
              neam::klmb::yaggler::no_textures
            >
            // CONTEXT
            (
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
             neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative)
            ));

            elements_smgr.materials.emplace_back(neam::klmb::yaggler::create_material_ptr
            <
              // SHADERS
              neam::klmb::yaggler::shader_list
              <
                neam::klmb::yaggler::auto_file_shader<dragon_vert>,
                neam::klmb::yaggler::gbuffer::gbuffer_geom_shader,
                neam::klmb::yaggler::gbuffer::gbuffer_frag_shader,
                neam::klmb::yaggler::auto_file_shader<mat5_frag>
              >,
              // TEXTURES
              neam::klmb::yaggler::no_textures
            >
            // CONTEXT
            (
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
              neam::klmb::yaggler::make_ctx_pair("view_matrix", neam::cr::make_const_ref(camera.view_matrix)),
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
              neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative)
            ));
            elements_smgr.materials.emplace_back(neam::klmb::yaggler::create_material_ptr
            <
              // SHADERS
              neam::klmb::yaggler::shader_list
              <
                neam::klmb::yaggler::auto_file_shader<mat6_vert>,
                neam::klmb::yaggler::gbuffer::gbuffer_geom_shader,
                neam::klmb::yaggler::gbuffer::gbuffer_frag_shader,
                neam::klmb::yaggler::auto_file_shader<mat6_frag>
              >,
              // TEXTURES
              neam::klmb::yaggler::no_textures
            >
            // CONTEXT
            (
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
              neam::klmb::yaggler::make_ctx_pair("view_matrix", neam::cr::make_const_ref(camera.view_matrix)),
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
              neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative)
            ));
            elements_smgr.materials.emplace_back(neam::klmb::yaggler::create_material_ptr
            <
              // SHADERS
              neam::klmb::yaggler::shader_list
              <
                neam::klmb::yaggler::auto_file_shader<dragon_vert>,
                neam::klmb::yaggler::gbuffer::gbuffer_geom_shader,
                neam::klmb::yaggler::gbuffer::gbuffer_frag_shader,
                neam::klmb::yaggler::auto_file_shader<mat7_frag>
              >,
              // TEXTURES
              neam::klmb::yaggler::no_textures
            >
            // CONTEXT
            (
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
              neam::klmb::yaggler::make_ctx_pair("view_matrix", neam::cr::make_const_ref(camera.view_matrix)),
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
              neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative)
            ));
            elements_smgr.materials.emplace_back(neam::klmb::yaggler::create_material_ptr
            <
              // SHADERS
              neam::klmb::yaggler::shader_list
              <
                neam::klmb::yaggler::auto_file_shader<dragon_vert>,
                neam::klmb::yaggler::gbuffer::gbuffer_geom_shader,
                neam::klmb::yaggler::gbuffer::gbuffer_frag_shader,
                neam::klmb::yaggler::auto_file_shader<mat8_frag>
              >,
              // TEXTURES
              neam::klmb::yaggler::no_textures
            >
            // CONTEXT
            (
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
              neam::klmb::yaggler::make_ctx_pair("view_matrix", neam::cr::make_const_ref(camera.view_matrix)),
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
              neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative)
            ));
            elements_smgr.materials.emplace_back(neam::klmb::yaggler::create_material_ptr
            <
              // SHADERS
              neam::klmb::yaggler::shader_list
              <
                neam::klmb::yaggler::auto_file_shader<dragon_vert>,
                neam::klmb::yaggler::gbuffer::gbuffer_geom_shader,
                neam::klmb::yaggler::gbuffer::gbuffer_frag_shader,
                neam::klmb::yaggler::auto_file_shader<mat9_frag>
              >,
              // TEXTURES
              neam::klmb::yaggler::no_textures
            >
            // CONTEXT
            (
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
              neam::klmb::yaggler::make_ctx_pair("view_matrix", neam::cr::make_const_ref(camera.view_matrix)),
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
              neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative)
            ));
            // editor shaders
            selected_material = neam::klmb::yaggler::create_material_ptr
            <
              // SHADERS
              neam::klmb::yaggler::shader_list
              <
                neam::klmb::yaggler::auto_file_shader<selected_vert>,
                neam::klmb::yaggler::auto_file_shader<selected_geom>,
                neam::klmb::yaggler::gbuffer::gbuffer_geom_shader,
                neam::klmb::yaggler::gbuffer::gbuffer_frag_shader,
                neam::klmb::yaggler::auto_file_shader<selected_frag>
              >,
              // TEXTURES
              neam::klmb::yaggler::no_textures
            >
            // CONTEXT
            (
              neam::klmb::yaggler::make_ctx_pair("screen_resolution", neam::cr::make_const_ref(framebuffer_resolution)),
              neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative)
            );
          }

        private:
          neam::klmb::yaggler::camera camera;
          glm::vec2 camera_movement = 0_vec2_xy;
          neam::bleunw::yaggler::scene::manager<editor_node> elements_smgr; // saved / restored smgr

          neam::klmb::yaggler::transformation_node::default_node *parent_camera_local_node;
          neam::klmb::yaggler::transformation_node::default_node *camera_local_node;

          neam::klmb::yaggler::gbuffer::gbuffer gbuffer;

          // app control
          bool do_quit = false;

          // editor stuff
          glm::vec3 obj_pos = 0_vec3_xyz;
          glm::vec3 mouse_ray_dir = -1_vec3_z;

          klmb::yaggler::material_wrapper selected_material;
          klmb::yaggler::material_wrapper old_material;

          editor_node *selected_node = nullptr;
          glm::vec3 move_direction = 0_vec3_xyz;
          glm::vec3 original_position = 0_vec3_xyz;
          glm::quat original_rotation = glm::quat();
          bool in_object_move = false;
          bool in_object_rot = false;

          // compositors controls
          double flash_time = -2.;
          glm::vec4 flash_color = 1_vec4_xyzw;

          float blur_max_distance = 200.f;
          float blur_center = 9.;
      };
    } // namespace sample
  } // namespace klmb
} // namespace neam

#endif /*__N_1657502776544798074_976431267__APP_HPP__*/

// implementations
#include "app_loader.hpp"
#include "app_events.hpp"

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

