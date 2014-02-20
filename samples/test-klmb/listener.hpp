//
// file : listener.hpp
// in : file:///home/tim/projects/yaggler/samples/test-klmb/listener.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 19/02/2014 23:13:31
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

#ifndef __N_1559079767981120197_2120402712__LISTENER_HPP__
# define __N_1559079767981120197_2120402712__LISTENER_HPP__

#include <bleunw/events.hpp>

namespace neam
{
  namespace klmb
  {
    namespace sample
    {
      class listener : public neam::bleunw::yaggler::events::listener
      {
        public:
          listener(bleunw::yaggler::events::manager &_emgr) : emgr(_emgr)
          {
            emgr.register_listener(this);
          }

          virtual ~listener()
          {
            emgr.unregister_listener(this);
          }

          virtual void button_pressed(const bleunw::yaggler::events::mouse_status &ms, bleunw::yaggler::events::mouse_buttons::mouse_buttons mb)
          {
            (void)ms;
            (void)mb;
          }

          virtual void button_released(const bleunw::yaggler::events::mouse_status &ms, bleunw::yaggler::events::mouse_buttons::mouse_buttons mb)
          {
            (void)ms;
            (void)mb;
          }

          virtual void key_pressed(const bleunw::yaggler::events::keyboard_status &ks, bleunw::yaggler::events::key_code::key_code kc)
          {
            (void)ks;
            (void)kc;
          }

          virtual void key_released(const bleunw::yaggler::events::keyboard_status &ks, bleunw::yaggler::events::key_code::key_code kc)
          {
            (void)ks;
            (void)kc;

            if (kc == bleunw::yaggler::events::key_code::escape)
              do_quit = true;
          }

          virtual void mouse_moved(const bleunw::yaggler::events::mouse_status &ms)
          {
            (void)ms;
          }

          virtual void on_input(const bleunw::yaggler::events::keyboard_status &ks, uint32_t code)
          {
            (void)ks;
            (void)code;
          }

          virtual void framebuffer_resized(const glm::vec2 &new_size)
          {
            (void)new_size;
          }

          virtual void window_closed()
          {
            do_quit = true;
          }

          virtual void window_focused(bool focused)
          {
            (void)focused;
          }

          virtual void window_iconified(bool iconified)
          {
            (void)iconified;
          }

          virtual void window_resized(const glm::vec2 &new_size)
          {
            (void)new_size;
          }

          virtual void window_content_refresh()
          {
          }

        public:
          bool do_quit = false;

        private:
          bleunw::yaggler::events::manager &emgr;
      };
    } // namespace sample
  } // namespace klmb
} // namespace neam

#endif /*__N_1559079767981120197_2120402712__LISTENER_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

