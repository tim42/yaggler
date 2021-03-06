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

#include <bleunw/application.hpp>

#include <klmb/klmb.hpp>


#define CRAP__VAR_TO_STRING(var)              static_cast<const std::ostringstream&>(std::ostringstream() << var).str()


constexpr neam::string_t test_frag = "samples_data/shaders/test/edge.frag";
constexpr neam::string_t yaggler_logo = "samples_data/yaggler.png";

namespace neam
{
  namespace klmb
  {
    namespace sample
    {
      class main_application : public neam::bleunw::yaggler::application::base_application<main_application>,
        neam::bleunw::yaggler::events::keyboard_listener
      {
        public:
          using default_transformation_tree_node_type = neam::klmb::yaggler::transformation_node::default_node;

        public:
          virtual ~main_application()
          {
            for (size_t i = 0; i < sizeof(worker) / sizeof(worker[0]); ++i)
              worker[i].join();
          }

          main_application()
            : base_application(neam::yaggler::glfw_window(neam::yaggler::window_mode::windowed, {980, 980}))
          {
            init();
            init_compositor();

            emgr.register_keyboard_listener(this);
            // emgr.register_mouse_listener(this); // unused in this demo

            // Load the task scheduler
            for (size_t i = 0; i < 4200; ++i)
            {
              auto &ctrl = scheduler.push_task([](float, neam::bleunw::yaggler::task::task_control &)
              {
                volatile size_t j = 0;
                volatile const size_t max = rand() % 15000;
                for (; j < max; ++j) {};
              }, (i % 5) ? neam::bleunw::yaggler::task::execution_type::normal : neam::bleunw::yaggler::task::execution_type::gl);
              ctrl.repeat = true;
              ctrl.delay = (i % 6) / 3.f;
            }

            // Launch a runner thread
            for (size_t i = 0; i < sizeof(worker) / sizeof(worker[0]); ++i)
            {
              worker[i] = std::thread([this]()
              {
                while (!this->do_quit)
                {
                  this->scheduler.wait_for_frame_end();
                  this->scheduler.run_some();
                }
              });
            }
            // Configure the scheduler to get the maximum throughput
            scheduler.assume_stable_second_thread(true);
          }

        private:
          void init()
          {
            // TEST GUI
            gmgr.fmgr.load_font("DejaVuSans", "samples_data/font/DejaVuSans.bfont");
            gmgr.fmgr.load_font("DejaVuSans-Oblique", "samples_data/font/DejaVuSans-Oblique.bfont");
            gmgr.fmgr.load_font("DejaVuSans-Bold", "samples_data/font/DejaVuSans-Bold.bfont");

            gmgr.add_renderable(&fps_gui_text);
            gmgr.add_renderable(&random_gui_text);

            fps_gui_text.set_font(gmgr.fmgr.get_font_ptr("DejaVuSans"));
            random_gui_text.set_font(gmgr.fmgr.get_font_ptr("DejaVuSans-Oblique"));

            auto &text_node = gmgr.transformation_tree.root.create_child();
            text_local_node = text_node.local;
            text_node.local->position = 0.07_vec3_y;
            text_node.local->scale = 0.05_vec3_xyz;
            text_node.local->dirty = true;

            auto &ch_text_node = text_node.create_child();
            ch_text_node.local->position = -0.10_vec3_y +0.1_vec3_x;
            ch_text_node.local->scale = 0.5_vec3_xyz;
            ch_text_node.local->dirty = true;

            // links
            fps_gui_text.world_pos = &text_node.world->matrix;
            fps_gui_text.color = glm::vec4(1., 1., 1., 0.85f);
            random_gui_text.world_pos = &ch_text_node.world->matrix;
            random_gui_text.color = glm::vec4(1., 0.9, 0.5, 1.f);

            gmgr.update();

            // yay !: gl calls.
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // register the task to update the text
            auto *ctrl = &scheduler.push_task([this](float, neam::bleunw::yaggler::task::task_control &)
            {
              fps_gui_text.set_text("fps: " + CRAP__VAR_TO_STRING(std::fixed << std::setprecision(1) << 1000.0 / get_imediate_mpf()));
              random_gui_text.set_text("mps: " + CRAP__VAR_TO_STRING(std::fixed << std::setprecision(2) << get_imediate_mpf() << "\n" << "time: " << cr::chrono::now_relative() 
                                       << "\nlateness: " << std::setprecision(4) << scheduler.get_lateness()));
            }, neam::bleunw::yaggler::task::execution_type::gl);
            ctrl->repeat = true;

            // Sample asynchronous update task
            ctrl = &scheduler.push_task([this](float, neam::bleunw::yaggler::task::task_control &)
            {
              fps_gui_text.color.g = glm::clamp(1.f - scheduler.get_lateness(), 0.2f, 1.f);
              fps_gui_text.color.b = fps_gui_text.color.g;

              std::lock_guard<neam::spinlock> _u0(text_local_node->lock);
              text_local_node->scale.x = 0.05f + scheduler.get_lateness() * 0.05f;
              text_local_node->scale.y = text_local_node->scale.x;
              text_local_node->scale.z = text_local_node->scale.x;
              // text_local_node->rotation = glm::rotate<float>(text_local_node->rotation, M_PI / 10.f * dt * 2.f, glm::vec3(1, 0, 0.1));
              text_local_node->dirty = true;
            });
            ctrl->repeat = true;
          }

          void init_compositor()
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

            compositor = std::move(simple_compositor);

            // compositor render task
            auto *ctrl = &scheduler.push_task([this](float, neam::bleunw::yaggler::task::task_control &)
            {
              compositor.render();
            }, neam::bleunw::yaggler::task::execution_type::pre_buffer_swap);
            ctrl->repeat = true;
            ctrl->priority = neam::bleunw::yaggler::application::scene_priority + 1;

          }

          virtual void key_pressed(const bleunw::yaggler::events::keyboard_status &ks, bleunw::yaggler::events::key_code::key_code kc)
          {
            if (ks.modifiers == bleunw::yaggler::events::modifier_keys::none && kc == bleunw::yaggler::events::key_code::escape)
              do_quit = true;
          }

          virtual void key_released(const bleunw::yaggler::events::keyboard_status &, bleunw::yaggler::events::key_code::key_code) {}

        private:
          neam::bleunw::yaggler::gui::text fps_gui_text;
          neam::bleunw::yaggler::gui::text random_gui_text;

          neam::klmb::yaggler::transformation_node::default_node *text_local_node;
          neam::klmb::yaggler::compositor_wrapper compositor;

          std::thread worker[1];
      };
    } // namespace sample
  } // namespace klmb
} // namespace neam

#endif /*__N_1657502776544798074_976431267__APP_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

