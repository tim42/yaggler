//
// file : application.hpp
// in : file:///home/tim/projects/yaggler/samples/test-klmb/application.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 20/02/2014 09:42:03
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

#ifndef __N_253284889691156201_892634093__APPLICATION_HPP__
# define __N_253284889691156201_892634093__APPLICATION_HPP__

#include <yaggler_glfw_window.hpp>

#include <bleunw/events.hpp>
#include <bleunw/scene.hpp>
#include <tools/chrono.hpp>
#include <iomanip>

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace application
      {
        // CRTP class
        // the child class must define the type 'default_transformation_tree_node_type'
        template<typename ChildClass>
        class base_application : private events::window_listener
        {
          public:
            base_application(neam::yaggler::glfw_window &&_window)
              : window(std::move(_window)), emgr(window), framebuffer_resolution(_framebuffer_resolution), mps_1s(0), mps_immediate(0), frame_counter(0)
            {
              _framebuffer_resolution.x = window.get_framebuffer_size().x;
              _framebuffer_resolution.y = window.get_framebuffer_size().y;

              emgr.register_window_listener(this);
            }

            virtual ~base_application()
            {
              emgr.unregister_window_listener(this);
            }

          protected:
            // should be called at the end of the render loop
            void end_render_loop()
            {
              constexpr double max_acc_time = 2.5;

              glfwPollEvents();
              window.swap_buffers();
              glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

              mps_immediate = 1000.0 * immed_chrono.delta();

              ++frame_counter;
              if (chrono.get_accumulated_time() >= max_acc_time)
              {
                mps_1s = max_acc_time * 1000.0 / static_cast<double>(frame_counter);
                frame_counter = 0;
                chrono.reset();

// #ifndef YAGGLER_NO_MESSAGES
                neam::cr::out.debug() << LOGGER_INFO << "f/s: "       << std::setw(9) << std::left <<  1000.0 / mps_1s
                          << "  |  ms/f: " << std::setw(9) << std::left << mps_1s
                          << std::endl;
// #endif
              }
            }

            // return milliseconds per frames (averaged on 1s)
            double get_mps() const
            {
              return mps_1s;
            }

            double get_imediate_mps() const
            {
              return mps_immediate;
            }

            // return frames per seconds (averaged on 1s)
            double get_fps() const
            {
              return 1000.0 / mps_1s;
            }

          protected:
            neam::yaggler::glfw_window window;
            neam::bleunw::yaggler::events::manager emgr;

            neam::bleunw::yaggler::scene::manager<> main_smgr;

            // used in shaders
            const glm::vec2 &framebuffer_resolution;

          protected:
            virtual void framebuffer_resized(const glm::vec2 &size) override
            {
              if (main_smgr.camera_holder._get_std_cam())
                main_smgr.camera_holder._get_std_cam()->aspect = static_cast<float>(size.x / size.y);
              _framebuffer_resolution = size;
            }

            virtual void window_closed() {}
            virtual void window_focused(bool ) {}
            virtual void window_iconified(bool ) {}
            virtual void window_resized(const glm::vec2 &) {}

          private:
            glm::vec2 _framebuffer_resolution;
            cr::chrono chrono;
            cr::chrono immed_chrono;

            double mps_1s;
            double mps_immediate;
            size_t frame_counter;
        };

      } // namespace application
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_253284889691156201_892634093__APPLICATION_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 


