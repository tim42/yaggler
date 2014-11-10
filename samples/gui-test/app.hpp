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

#include <iomanip>

#include "io.hpp"
#include <bleunw/application.hpp>

#include <bleunw/gui.hpp>

#include <klmb/klmb.hpp>
#include "loader.hpp"


#define CRAP__VAR_TO_STRING(var)              static_cast<std::ostringstream&>(std::ostringstream() << var).str()


constexpr neam::string_t test_frag = "data/shaders/test/edge.frag";
constexpr neam::string_t yaggler_logo = "data/yaggler.png";

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
//             : base_application(neam::yaggler::glfw_window(neam::yaggler::window_mode::fullscreen)),
            : base_application(neam::yaggler::glfw_window(neam::yaggler::window_mode::windowed, {980, 980})),
              gmgr(framebuffer_resolution, emgr)
          {
            init();
            emgr.register_keyboard_listener(this);
//             emgr.register_mouse_listener(this); // unused/able in this demo
          }

          void run()
          {
            // the bg renderer
            auto simple_compositor = neam::klmb::yaggler::make_compositor(neam::klmb::yaggler::make_framebuffer_pack
            (
              neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl>(0) // output to screen
            ));
            simple_compositor.add_pass<neam::klmb::yaggler::auto_file_shader<test_frag>>(0,
                neam::klmb::yaggler::input_texture_indexes<>(),

                neam::klmb::yaggler::make_ctx_pair("screen_resolution", cr::make_ref(framebuffer_resolution)),
                neam::klmb::yaggler::make_ctx_pair("global_time", &neam::cr::chrono::now_relative)
            );

            end_render_loop();


            cr::chrono chrono;

            while (!window.should_close() && !do_quit)
            {
              glViewport(0, 0, framebuffer_resolution.x, framebuffer_resolution.y);

              main_smgr.render();

              simple_compositor.render();

//               text_local_node->rotation = glm::rotate<float>(text_local_node->rotation, M_PI / 10.f * chrono.delta(), glm::vec3(0, 1, 1));
//               text_local_node->dirty = true;

              // disable depth test for THIS 3D text rendering. (only in this case: we render over a fullscreen quad in Z = 0)
              glDisable(GL_DEPTH_TEST);

              fps_gui_text.set_text("fps: " + CRAP__VAR_TO_STRING(std::fixed << std::setprecision(1) << 1000.0 / get_imediate_mps()));
              random_gui_text.set_text("mps: " + CRAP__VAR_TO_STRING(std::fixed << std::setprecision(2) << get_imediate_mps() << "\n" << "time: " << cr::chrono::now_relative()));

              gmgr.render();

              // re-enable depth test
              glEnable(GL_DEPTH_TEST);

              // end that loop
              end_render_loop();
            }
          }

        private:
          void init()
          {
            // TEST GUI
            gmgr.fmgr.load_font("DejaVuSans", "data/font/DejaVuSans.bfont");
            gmgr.fmgr.load_font("DejaVuSans-Oblique", "data/font/DejaVuSans-Oblique.bfont");
            gmgr.fmgr.load_font("DejaVuSans-Bold", "data/font/DejaVuSans-Bold.bfont");

            gmgr.add_renderable(&fps_gui_text);
            gmgr.add_renderable(&random_gui_text);

            fps_gui_text.set_font(gmgr.fmgr.get_font_ptr("DejaVuSans"));
            random_gui_text.set_font(gmgr.fmgr.get_font_ptr("DejaVuSans-Oblique"));

            auto &text_node = gmgr.transformation_tree.root.create_child();
            text_local_node = text_node.local;
            text_node.local->position = 0.05_vec3_y;
            text_node.local->scale = 0.05_vec3_xyz;
            text_node.local->dirty = true;

            auto &ch_text_node = text_node.create_child();
            ch_text_node.local->position = -0.10_vec3_y +0.1_vec3_x;
            ch_text_node.local->scale = 0.5_vec3_xyz;
            ch_text_node.local->dirty = true;

            // links
            fps_gui_text.world_pos = &text_node.world->matrix;
            fps_gui_text.color = glm::vec4(1., 1., 1., 0.5f);
            random_gui_text.world_pos = &ch_text_node.world->matrix;
            random_gui_text.color = glm::vec4(1., 0.9, 0.5, 1.f);

            // yay: gl calls :D
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          }

          virtual void button_pressed(const bleunw::yaggler::events::mouse_status &ms, bleunw::yaggler::events::mouse_buttons::mouse_buttons mb);
          virtual void button_released(const bleunw::yaggler::events::mouse_status &ms, bleunw::yaggler::events::mouse_buttons::mouse_buttons mb);
          virtual void mouse_moved(const bleunw::yaggler::events::mouse_status &ms);

          virtual void key_pressed(const bleunw::yaggler::events::keyboard_status &ks, bleunw::yaggler::events::key_code::key_code kc);
          virtual void key_released(const bleunw::yaggler::events::keyboard_status &ks, bleunw::yaggler::events::key_code::key_code kc);

        private:
          neam::bleunw::yaggler::gui::text fps_gui_text;
          neam::bleunw::yaggler::gui::text random_gui_text;
          neam::bleunw::yaggler::gui::manager gmgr;

          neam::klmb::yaggler::transformation_node::default_node *parent_camera_local_node;
          neam::klmb::yaggler::transformation_node::default_node *camera_local_node;
          neam::klmb::yaggler::transformation_node::default_node *text_local_node;

          // app control
          bool do_quit = false;
      };
    } // namespace sample
  } // namespace klmb
} // namespace neam

#endif /*__N_1657502776544798074_976431267__APP_HPP__*/

// implementations
#include "app_events.hpp"

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

