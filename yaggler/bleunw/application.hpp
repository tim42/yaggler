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
#include <bleunw/task.hpp>
#include <bleunw/gui.hpp>
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
        // constants
        constexpr int gui_priority = 100;
        constexpr int scene_priority = 50;
        constexpr int update_priority = 0;


        /// \brief Application base-class
        /// \note the child class must define the type 'default_transformation_tree_node_type'
        template<typename ChildClass>
        class base_application : private events::window_listener
        {
          public:
            /// \brief Initialize the application from a window
            base_application(neam::yaggler::glfw_window &&_window)
              : window(std::move(_window)), emgr(window), framebuffer_resolution(_framebuffer_resolution),
                gmgr(framebuffer_resolution, emgr), mpf_1s(0), mpf_immediate(0), frame_counter(0)
            {
              _framebuffer_resolution.x = window.get_framebuffer_size().x;
              _framebuffer_resolution.y = window.get_framebuffer_size().y;

              gmgr.framebuffer_resized(framebuffer_resolution);
              emgr.register_window_listener(this);
            }

            /// \brief destructor
            virtual ~base_application()
            {
              emgr.unregister_window_listener(this);
            }

            /// \brief Default run method, could be overrided if needed
            virtual void run()
            {
              register_update_and_render_tasks();

              while (!window.should_close() && !do_quit)
              {
                glViewport(0, 0, framebuffer_resolution.x, framebuffer_resolution.y);

                // run the tasks
                scheduler.run_some();

                // end that loop
                end_render_loop();

                if (get_fps() < 30.f) // We got a poor framerate, try to release some CPU for the rendering
                  scheduler.enforce_task_repartition(true);
                else if (get_fps() > 40.f && scheduler.is_late(0.7)) // We got some lateness recently, optimize for throughput
                  scheduler.enforce_task_repartition(false);
              }
              do_quit = true;

              // cleanup the scheduler (we have tasks that reference local elements)
              // after the call, all threads will be locked in the wait_for_frame_end() method
              scheduler.clear();
              // so we just release them by calling end_frame(), as do_quit is set, all the threads will exit
              scheduler.end_frame();
            }

          protected:
            /// \brief End the current frame
            /// \note should ALWAYS be called at the end of the render loop as it call scheduler.end_frame()
            void end_render_loop()
            {
              constexpr double max_acc_time = 2.5;

              scheduler.end_frame(); // sync the scheduler

              glfwPollEvents();
              window.swap_buffers();
              glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

              mpf_immediate = 1000.0 * immed_chrono.delta();

              ++frame_counter;
              if (chrono.get_accumulated_time() >= max_acc_time)
              {
                mpf_1s = max_acc_time * 1000.0 / static_cast<double>(frame_counter);
                frame_counter = 0;
                chrono.reset();

#ifndef YAGGLER_NO_MESSAGES
                neam::cr::out.debug() << LOGGER_INFO << "f/s: "       << std::setw(9) << std::left <<  1000.0 / mpf_1s
                          << "  |  ms/f: " << std::setw(9) << std::left << mpf_1s
                          << std::endl;
#endif
              }
            }

            /// \brief return milliseconds per frames (averaged on 1s)
            double get_mpf() const
            {
              return mpf_1s;
            }

            /// \brief Return the last frame milliseconds per frame
            double get_imediate_mpf() const
            {
              return mpf_immediate;
            }

            /// \brief Return frames per seconds (averaged on 1s)
            double get_fps() const
            {
              return 1000.0 / mpf_1s;
            }

            /// \brief Register the scene manager / gui manager render & update tasks
            void register_update_and_render_tasks()
            {
              // register the task to update the tr-trees of both the scene and the GUI
              auto *ctrl = &scheduler.push_task([this](float, neam::bleunw::yaggler::task::task_control &)
              {
                main_smgr.update();
                gmgr.update();
              });
              ctrl->repeat = true;
              ctrl->priority = update_priority;

              // Render tasks
              ctrl = &scheduler.push_task([this](float, neam::bleunw::yaggler::task::task_control &)
              {
                main_smgr.render();
              }, neam::bleunw::yaggler::task::execution_type::pre_buffer_swap);
              ctrl->repeat = true;
              ctrl->priority = scene_priority;

              ctrl = &scheduler.push_task([this](float, neam::bleunw::yaggler::task::task_control &)
              {
                // disable depth test for THIS 3D text rendering. (only in this case: we render over a fullscreen quad in Z = 0)
                glDisable(GL_DEPTH_TEST);

                gmgr.render();

                // re-enable depth test
                glEnable(GL_DEPTH_TEST);
              }, neam::bleunw::yaggler::task::execution_type::pre_buffer_swap);
              ctrl->repeat = true;
              ctrl->priority = gui_priority;
            }

          protected:
            neam::yaggler::glfw_window window; ///< \brief the window of the application
            neam::bleunw::yaggler::events::manager emgr; ///< \brief The event manager of the application

            neam::bleunw::yaggler::scene::manager<> main_smgr; ///< \brief the main scene manager of the application

            // used in shaders
            const glm::vec2 &framebuffer_resolution; ///< \brief The current framebuffer resolution

            neam::bleunw::yaggler::gui::manager gmgr; ///< \brief The GUI manager

            neam::bleunw::yaggler::task::scheduler scheduler; ///< \brief A default task scheduler for the application

            bool do_quit = false; ///< \brief Exit flag (simply set it to true to end the render loop)

          protected:
            /// \brief Called when the framebuffer is resized
            virtual void framebuffer_resized(const glm::vec2 &size) override
            {
              if (main_smgr.camera_holder._get_std_cam())
                main_smgr.camera_holder._get_std_cam()->aspect = static_cast<float>(size.x / size.y);
              _framebuffer_resolution = size;
            }

            virtual void window_closed() override {}
            virtual void window_focused(bool ) override {}
            virtual void window_iconified(bool ) override {}
            virtual void window_resized(const glm::vec2 &) override {}

          private:
            glm::vec2 _framebuffer_resolution;
            cr::chrono chrono;
            cr::chrono immed_chrono;

            double mpf_1s;
            double mpf_immediate;
            size_t frame_counter;
        };

      } // namespace application
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_253284889691156201_892634093__APPLICATION_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 


