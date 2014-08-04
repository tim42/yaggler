//
// file : gui.hpp
// in : file:///home/tim/projects/yaggler/yaggler/bleunw/gui.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 22/02/2014 22:27:18
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

#ifndef __N_702277851988284911_350494955__GUI_HPP__
# define __N_702277851988284911_350494955__GUI_HPP__

#include <deque>
#include <list>

#include <bleunw/gui/renderable.hpp>
#include <bleunw/gui/font_face.hpp>
#include <bleunw/gui/font_manager.hpp>
#include <bleunw/gui/text.hpp>
// #include <bleunw/gui/layout_descriptor.hpp>
// #include <bleunw/gui/tr_node.hpp>
#include <bleunw/gui/image.hpp>

#include <bleunw/events/event_listener.hpp>
#include <bleunw/events.hpp>

#include <klmb/camera/camera.hpp>

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace gui
      {

        // this class implement some kind of fragmented array for O(1) insertion and suppression but delayed ids retrieving.
        // (this mean: you can't add and delete an object without calling the non-const 'render()')
        // (this also mean: if you call the const 'render()' you'll render the gui, but without adding/rendering any new elements in the manager,
        //  allowing to generate changes in multiple frames and batch them in a single frame. This also allow to render an unmodified version of the GUI multiple times if changes are costly)
        class manager : public renderable, public events::window_listener // see what could be done ;)
        {
          public:
            manager(const manager &) = delete;
            manager &operator =(const manager &) = delete;

            manager(const glm::vec2 &framebuffer_resolution, events::manager &_emgr) : emgr(_emgr)
            {
              framebuffer_resized(framebuffer_resolution);

              internal_vp_ptr = &camera.vp_matrix;
              vp_matrix = &internal_vp_ptr;
              world_pos = &internal_world_mat;

              emgr.register_window_listener(this);
            }

            ~manager()
            {
              emgr.unregister_window_listener(this);
            }

            // ghdl will held the handle to this renderable and will be set to the correct value after a call to the non-const render() method.
            // you should set it to &(rd->handle) (and that's the default, when ghdl is == to nullptr)
            void add_renderable(renderable *rd, handle_t *ghdl = nullptr)
            {
              rd->vp_matrix = vp_matrix;

              if (!ghdl)
                ghdl = &rd->handle;
              *ghdl = -1;
              to_add.push_back({rd, ghdl});
              ++count;
            }

            void remove_renderable(handle_t rd)
            {
              if (rd < elements.size() && count)
              {
                elements[rd] = nullptr;
                --count;
              }
            }

            virtual void render() final
            {
              size_t i = 0;
              size_t last = 0;

              transformation_tree.root.recompute_matrices();

              for (; i < elements.size(); ++i)
              {
                if (!elements[i] && to_add.size())
                {
                  elements[i] = to_add.front().first;
                  *(to_add.front().second) = i;
                  to_add.pop_front();
                }

                if (elements[i])
                {
                  last = i;
                  elements[i]->render();
                }
              }
              while (to_add.size())
              {
                to_add.front().first->render();
                *(to_add.front().second) = i;
                elements.push_back(to_add.front().first);
                to_add.pop_front();
                last = i;
                ++i;
              }

              // remove trailing empty slots
              if (elements.size() > 5 && last < elements.size() - 5)
                elements.erase(elements.begin() + last, elements.end());
            }

            // prevent any modification (no add will be in effect here).
            virtual void render() const final
            {
              for (size_t i = 0; i < elements.size(); ++i)
              {
                if (elements[i])
                  elements[i]->render();
              }
            }

          public: // window_listener
            virtual void framebuffer_resized(const glm::vec2 &framebuffer_resolution)
            {
//               camera.min = glm::vec2(-1, -framebuffer_resolution.y / framebuffer_resolution.x);
//               camera.max = glm::vec2(framebuffer_resolution.x / framebuffer_resolution.y, 1);
              camera.min = glm::vec2(0, 0);
              camera.max = glm::vec2(1.f + framebuffer_resolution.x / framebuffer_resolution.y, 1.f + framebuffer_resolution.y / framebuffer_resolution.x);
              camera.recompute_matrices();

              // dirty the root (but do not recompute matrices here).
              transformation_tree.root.local->dirty = true;
            }

            virtual void window_closed() {}
            virtual void window_focused(bool) {}
            virtual void window_iconified(bool) {}
            virtual void window_resized(const glm::vec2 &) {}

          public:
            // the font manager
            font_manager fmgr;
            neam::klmb::yaggler::transformation_tree</*transformation_node::gui_node*/klmb::yaggler::transformation_node::default_node> transformation_tree;

          private:
            events::manager &emgr;
            neam::klmb::yaggler::ortho_camera camera;
            glm::mat4 *internal_vp_ptr;
            glm::mat4 internal_world_mat;

            std::deque<renderable *> elements;
            std::list<std::pair<renderable *, handle_t *>> to_add;
            size_t count;
        };
      } // namespace gui
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_702277851988284911_350494955__GUI_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

