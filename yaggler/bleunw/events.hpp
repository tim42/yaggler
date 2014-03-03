//
// file : manager.hpp
// in : file:///home/tim/projects/yaggler/yaggler/bleunw/manager.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 19/02/2014 21:35:43
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

#ifndef __N_6409777231508981750_1688087466__EVENT_MANAGER_HPP__
# define __N_6409777231508981750_1688087466__EVENT_MANAGER_HPP__

#include <set>

#include <bleunw/events/event.hpp>
#include <bleunw/events/event_listener.hpp>
#include <yaggler_glfw_window.hpp>

namespace neam
{
  namespace bleunw
  {
    namespace yaggler
    {
      namespace events
      {
        class manager
        {
          public:
            manager(neam::yaggler::glfw_window &_win) : win(_win)
            {
              // register glfw callbacks and user pointer
              glfwSetWindowUserPointer(win._get_glfw_handle(), this);

              glfwSetKeyCallback(win._get_glfw_handle(), &manager::_t_key);
              glfwSetCharCallback(win._get_glfw_handle(), &manager::_t_unicode_input);
              glfwSetMouseButtonCallback(win._get_glfw_handle(), &manager::_t_mouse_button);
              glfwSetCursorPosCallback(win._get_glfw_handle(), &manager::_t_mouse_move);
              glfwSetScrollCallback(win._get_glfw_handle(), &manager::_t_mouse_wheel);

              glfwSetWindowSizeCallback(win._get_glfw_handle(), &manager::_t_window_size);
              glfwSetWindowFocusCallback(win._get_glfw_handle(), &manager::_t_focus);
              glfwSetWindowIconifyCallback(win._get_glfw_handle(), &manager::_t_iconify);
              glfwSetWindowPosCallback(win._get_glfw_handle(), &manager::_t_window_pos);
              glfwSetWindowRefreshCallback(win._get_glfw_handle(), &manager::_t_refresh);
              glfwSetWindowCloseCallback(win._get_glfw_handle(), &manager::_t_close);
              glfwSetFramebufferSizeCallback(win._get_glfw_handle(), &manager::_t_buffer_resize);

              // init the mouse state
              last_mouse_status.buttons = events::mouse_buttons::none;
              double _cpos[2];
              glfwGetCursorPos(win._get_glfw_handle(), _cpos, _cpos + 1);
              last_mouse_status.position.x = _cpos[0];
              last_mouse_status.position.y = _cpos[1];

              int _wsz[2];
              glfwGetWindowSize(win._get_glfw_handle(), _wsz, _wsz + 1);
              glm::vec2 wsz(_wsz[0], _wsz[1]);

              last_mouse_status.normalized_position = last_mouse_status.position / wsz;
              last_mouse_status.modifiers = events::modifier_keys::none;
              last_mouse_status.wheel = glm::vec2(0, 0);
            }

            ~manager()
            {
              glfwSetWindowUserPointer(win._get_glfw_handle(), nullptr);

              // unregister callbacks
              glfwSetKeyCallback(win._get_glfw_handle(), nullptr);
              glfwSetCharCallback(win._get_glfw_handle(), nullptr);
              glfwSetMouseButtonCallback(win._get_glfw_handle(), nullptr);
              glfwSetCursorPosCallback(win._get_glfw_handle(), nullptr);
              glfwSetScrollCallback(win._get_glfw_handle(), nullptr);

              glfwSetWindowSizeCallback(win._get_glfw_handle(), nullptr);
              glfwSetWindowFocusCallback(win._get_glfw_handle(), nullptr);
              glfwSetWindowIconifyCallback(win._get_glfw_handle(), nullptr);
              glfwSetWindowPosCallback(win._get_glfw_handle(), nullptr);
              glfwSetWindowRefreshCallback(win._get_glfw_handle(), nullptr);
              glfwSetWindowCloseCallback(win._get_glfw_handle(), nullptr);
              glfwSetFramebufferSizeCallback(win._get_glfw_handle(), nullptr);
            }

            void register_mouse_listener(events::mouse_listener *ml)
            {
              mlisteners.insert(ml);
            }

            void unregister_mouse_listener(events::mouse_listener *ml)
            {
              mlisteners.erase(ml);
            }

            void register_keyboard_listener(events::keyboard_listener *kl)
            {
              klisteners.insert(kl);
            }

            void unregister_keyboard_listener(events::keyboard_listener *kl)
            {
              klisteners.erase(kl);
            }

            void register_window_listener(events::window_listener *wl)
            {
              wlisteners.insert(wl);
            }

            void unregister_window_listener(events::window_listener *wl)
            {
              wlisteners.erase(wl);
            }

            void register_listener(events::listener *l)
            {
              mlisteners.insert(l);
              klisteners.insert(l);
              wlisteners.insert(l);
            }

            void unregister_listener(events::listener *l)
            {
              mlisteners.erase(l);
              klisteners.erase(l);
              wlisteners.erase(l);
            }

            // delete them all
            manager(const manager &) = delete;
            manager(manager &&) = delete;
            manager &operator = (const manager &) = delete;
            manager &operator = (manager &&) = delete;

          private: // trampos
            static void _t_mouse_button(GLFWwindow *glfw_win, int button, int action, int modifers)
            {
              manager *emgr = reinterpret_cast<manager *>(glfwGetWindowUserPointer(glfw_win));
              emgr->_mouse_button(glfw_win, button, action, modifers);
            }

            static void _t_mouse_wheel(GLFWwindow *glfw_win, double x, double y)
            {
              manager *emgr = reinterpret_cast<manager *>(glfwGetWindowUserPointer(glfw_win));
              emgr->_mouse_wheel(glfw_win, x, y);
            }

            static void _t_mouse_move(GLFWwindow *glfw_win, double x, double y)
            {
              manager *emgr = reinterpret_cast<manager *>(glfwGetWindowUserPointer(glfw_win));
              emgr->_mouse_move(glfw_win, x, y);
            }

            static void _t_key(GLFWwindow *glfw_win, int key, int scancode, int action, int modifers)
            {
              manager *emgr = reinterpret_cast<manager *>(glfwGetWindowUserPointer(glfw_win));
              emgr->_key(glfw_win, key, scancode, action, modifers);
            }

            static void _t_unicode_input(GLFWwindow *glfw_win, unsigned int code)
            {
              manager *emgr = reinterpret_cast<manager *>(glfwGetWindowUserPointer(glfw_win));
              emgr->_unicode_input(glfw_win, code);
            }

            static void _t_window_pos(GLFWwindow *glfw_win, int x, int y)
            {
              manager *emgr = reinterpret_cast<manager *>(glfwGetWindowUserPointer(glfw_win));
              emgr->_window_pos(glfw_win, x, y);
            }

            static void _t_window_size(GLFWwindow *glfw_win, int x, int y)
            {
              manager *emgr = reinterpret_cast<manager *>(glfwGetWindowUserPointer(glfw_win));
              emgr->_window_size(glfw_win, x, y);
            }

            static void _t_close(GLFWwindow *glfw_win)
            {
              manager *emgr = reinterpret_cast<manager *>(glfwGetWindowUserPointer(glfw_win));
              emgr->_close(glfw_win);
            }

            static void _t_refresh(GLFWwindow *glfw_win)
            {
              manager *emgr = reinterpret_cast<manager *>(glfwGetWindowUserPointer(glfw_win));
              emgr->_refresh(glfw_win);
            }

            static void _t_focus(GLFWwindow *glfw_win, int focus)
            {
              manager *emgr = reinterpret_cast<manager *>(glfwGetWindowUserPointer(glfw_win));
              emgr->_focus(glfw_win, focus);
            }

            static void _t_iconify(GLFWwindow *glfw_win, int iconify)
            {
              manager *emgr = reinterpret_cast<manager *>(glfwGetWindowUserPointer(glfw_win));
              emgr->_iconify(glfw_win, iconify);
            }

            static void _t_buffer_resize(GLFWwindow *glfw_win, int x, int y)
            {
              manager *emgr = reinterpret_cast<manager *>(glfwGetWindowUserPointer(glfw_win));
              emgr->_buffer_resize(glfw_win, x, y);
            }

          private: // called by the trampos
            void _mouse_button(GLFWwindow *, int button, int action, int modifers)
            {
              events::mouse_status current_mouse_status = last_mouse_status;

              current_mouse_status.delta.normalized_position = glm::vec2(0, 0);
              current_mouse_status.delta.position = glm::vec2(0, 0);
              current_mouse_status.delta.wheel = glm::vec2(0, 0);
              current_mouse_status.delta.active_buttons = events::mouse_buttons::none;
              if (action == GLFW_PRESS)
                current_mouse_status.delta.active_buttons = static_cast<events::mouse_buttons::mouse_buttons>(1 << button);


              current_mouse_status.modifiers = static_cast<events::modifier_keys::modifier_keys>(modifers);
              if (action == GLFW_PRESS)
                current_mouse_status.buttons =  static_cast<events::mouse_buttons::mouse_buttons>(current_mouse_status.buttons | (1 << button));
              else if (action == GLFW_RELEASE)
                current_mouse_status.buttons = static_cast<events::mouse_buttons::mouse_buttons>(current_mouse_status.buttons & ~(1 << button));

              for (events::mouse_listener * ml : mlisteners)
              {
                if (action == GLFW_PRESS)
                  ml->button_pressed(current_mouse_status, current_mouse_status.delta.active_buttons);
                else if (action == GLFW_RELEASE)
                  ml->button_released(current_mouse_status, static_cast<events::mouse_buttons::mouse_buttons>(1 << button));
              }

              last_mouse_status = current_mouse_status;
            }

            void _mouse_wheel(GLFWwindow *, double x, double y)
            {
              events::mouse_status current_mouse_status = last_mouse_status;

              current_mouse_status.delta.normalized_position = glm::vec2(0, 0);
              current_mouse_status.delta.position = glm::vec2(0, 0);
              current_mouse_status.delta.wheel = glm::vec2(x, y);
              current_mouse_status.delta.active_buttons = events::mouse_buttons::none;


              current_mouse_status.wheel += glm::vec2(x, y);

              for (events::mouse_listener * ml : mlisteners)
              {
                ml->mouse_moved(current_mouse_status);
              }

              last_mouse_status = current_mouse_status;
            }

            void _mouse_move(GLFWwindow *glfw_win, double x, double y)
            {
              int _wsz[2];
              glfwGetWindowSize(glfw_win, _wsz, _wsz + 1);
              glm::vec2 wsz(_wsz[0], _wsz[1]);

              events::mouse_status current_mouse_status = last_mouse_status;

              current_mouse_status.delta.normalized_position = last_mouse_status.normalized_position - (glm::vec2(x, y) / wsz);
              current_mouse_status.delta.position = last_mouse_status.position - glm::vec2(x, y);
              current_mouse_status.delta.wheel = glm::vec2(0, 0);
              current_mouse_status.delta.active_buttons = events::mouse_buttons::none;


              current_mouse_status.normalized_position = (glm::vec2(x, y) / wsz);
              current_mouse_status.position = glm::vec2(x, y);

              for (events::mouse_listener * ml : mlisteners)
              {
                ml->mouse_moved(current_mouse_status);
              }

              last_mouse_status = current_mouse_status;
            }

            void _key(GLFWwindow *, int key, int , int action, int modifers)
            {
              events::keyboard_status ks;

              ks.modifiers = static_cast<events::modifier_keys::modifier_keys>(modifers);
              last_mouse_status.modifiers = ks.modifiers;

              events::key_code::key_code key_code = static_cast<events::key_code::key_code>(key);

              // mouse modifers
              switch (key_code)
              {
                case events::key_code::left_control:
                case events::key_code::right_control:
                  if (action == GLFW_PRESS)
                    last_mouse_status.modifiers = static_cast<events::modifier_keys::modifier_keys>(last_mouse_status.modifiers | events::modifier_keys::modifier_keys::control);
                  else
                    last_mouse_status.modifiers = static_cast<events::modifier_keys::modifier_keys>(last_mouse_status.modifiers & ~events::modifier_keys::modifier_keys::control);
                  break;
                case events::key_code::left_alt:
                case events::key_code::right_alt:
                  if (action == GLFW_PRESS)
                    last_mouse_status.modifiers = static_cast<events::modifier_keys::modifier_keys>(last_mouse_status.modifiers | events::modifier_keys::modifier_keys::alt);
                  else
                    last_mouse_status.modifiers = static_cast<events::modifier_keys::modifier_keys>(last_mouse_status.modifiers & ~events::modifier_keys::modifier_keys::alt);
                  break;
                case events::key_code::left_shift:
                case events::key_code::right_shift:
                  if (action == GLFW_PRESS)
                    last_mouse_status.modifiers = static_cast<events::modifier_keys::modifier_keys>(last_mouse_status.modifiers | events::modifier_keys::modifier_keys::shift);
                  else
                    last_mouse_status.modifiers = static_cast<events::modifier_keys::modifier_keys>(last_mouse_status.modifiers & ~events::modifier_keys::modifier_keys::shift);
                  break;
                case events::key_code::left_super:
                case events::key_code::right_super:
                  if (action == GLFW_PRESS)
                    last_mouse_status.modifiers = static_cast<events::modifier_keys::modifier_keys>(last_mouse_status.modifiers | events::modifier_keys::modifier_keys::super);
                  else
                    last_mouse_status.modifiers = static_cast<events::modifier_keys::modifier_keys>(last_mouse_status.modifiers & ~events::modifier_keys::modifier_keys::super);
                  break;
                default: // this makes gcc happy.
                  break;
              }

              for (events::keyboard_listener * kl : klisteners)
              {
                if (action == GLFW_PRESS)
                  kl->key_pressed(ks, key_code);
                else if (action == GLFW_RELEASE)
                  kl->key_released(ks, key_code);
              }
              last_keyboard_status = ks;
            }

            void _unicode_input(GLFWwindow *, unsigned int code)
            {
              for (events::keyboard_listener * kl : klisteners)
              {
                kl->on_input(last_keyboard_status, code);
              }
            }

            void _window_pos(GLFWwindow *, int x, int y)
            {
              glm::vec2 v(x, y);
//               for (events::window_listener * wl : wlisteners)
//               {
//               }
            }

            void _window_size(GLFWwindow *, int x, int y)
            {
              glm::vec2 v(x, y);
              for (events::window_listener * wl : wlisteners)
                wl->window_resized(v);
            }

            void _close(GLFWwindow *)
            {
              for (events::window_listener * wl : wlisteners)
                wl->window_closed();
            }

            void _refresh(GLFWwindow *)
            {
              for (events::window_listener * wl : wlisteners)
                wl->window_content_refresh();
            }

            void _focus(GLFWwindow *, int focus)
            {
              for (events::window_listener * wl : wlisteners)
                wl->window_focused(focus);
            }

            void _iconify(GLFWwindow *, int iconify)
            {
              for (events::window_listener * wl : wlisteners)
                wl->window_iconified(iconify);
            }

            void _buffer_resize(GLFWwindow *, int x, int y)
            {
              glm::vec2 v(x, y);
              for (events::window_listener * wl : wlisteners)
                wl->framebuffer_resized(v);
            }

          private:
            neam::yaggler::glfw_window &win;
            std::set<events::keyboard_listener *> klisteners;
            std::set<events::mouse_listener *> mlisteners;
            std::set<events::window_listener *> wlisteners;

            events::mouse_status last_mouse_status;
            events::keyboard_status last_keyboard_status;
        };
      } // namespace events
    } // namespace yaggler
  } // namespace bleunw
} // namespace neam

#endif /*__N_6409777231508981750_1688087466__EVENT_MANAGER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 


