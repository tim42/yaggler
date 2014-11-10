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

#include <bleunw/gui.hpp>

#include <klmb/klmb.hpp>


constexpr neam::string_t pt_vert = "data/shaders/path-tracer/path-tracer.vert";
constexpr neam::string_t pt_frag = "data/shaders/path-tracer/path-tracer.frag";

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
//             : base_application(neam::yaggler::glfw_window(neam::yaggler::window_mode::fullscreen))
            : base_application(neam::yaggler::glfw_window(neam::yaggler::window_mode::windowed, {1000, 1000}))
          {
            init();
            emgr.register_keyboard_listener(this);
//             emgr.register_mouse_listener(this); // unused/able in this demo
          }

          void run()
          {
            // the 3D image
            neam::yaggler::texture::texture<neam::yaggler::type::opengl, neam::embed::GLenum<GL_TEXTURE_3D>> light_texture;
            light_texture.set_image_texture(true, GL_RGBA32F, GL_READ_WRITE);

            int *img_data = new int[202*202*202*4];
            memset(img_data, 0, 4 * 202*202*202);
            light_texture.set_texture_data(GL_RGBA32F, ct::vector3(202, 202, 202), GL_RGBA, GL_BYTE, img_data);

            // the rendered
            auto path_tracer_compositor = neam::klmb::yaggler::make_compositor(neam::klmb::yaggler::make_framebuffer_pack
            (
              neam::yaggler::texture::framebuffer<neam::yaggler::type::opengl>(0), // output to screen

              // gbuffer textures
              neam::klmb::yaggler::make_texture_entry("light_space_map", light_texture, 0)
            ));
            path_tracer_compositor.add_pass<neam::klmb::yaggler::auto_file_shader<pt_frag>>(0,
                neam::klmb::yaggler::input_texture_indexes<0>(),

                neam::klmb::yaggler::make_ctx_pair("buffer_size", cr::make_ref(framebuffer_resolution))
            );

            end_render_loop();

            while (!window.should_close() && !do_quit)
            {
              glViewport(0, 0, framebuffer_resolution.x, framebuffer_resolution.y);

              path_tracer_compositor.render();

              end_render_loop();
            }
          }

        private:
          void init()
          {
          }

          virtual void button_pressed(const bleunw::yaggler::events::mouse_status &ms, bleunw::yaggler::events::mouse_buttons::mouse_buttons mb);
          virtual void button_released(const bleunw::yaggler::events::mouse_status &ms, bleunw::yaggler::events::mouse_buttons::mouse_buttons mb);
          virtual void mouse_moved(const bleunw::yaggler::events::mouse_status &ms);

          virtual void key_pressed(const bleunw::yaggler::events::keyboard_status &ks, bleunw::yaggler::events::key_code::key_code kc);
          virtual void key_released(const bleunw::yaggler::events::keyboard_status &ks, bleunw::yaggler::events::key_code::key_code kc);

        private:

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

