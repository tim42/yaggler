//
// file : app_events.hpp
// in : file:///home/tim/projects/yaggler/samples/test-klmb/app_events.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 28/02/2014 18:19:51
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

#ifndef __N_8334687671285212906_717271234__APP_EVENTS_HPP__
# define __N_8334687671285212906_717271234__APP_EVENTS_HPP__

#include "app.hpp"

// yep, everything is in the events receivers, sometimes code are duplicated...
// [GET IT DONE] :(

namespace neam
{
  namespace klmb
  {
    namespace sample
    {


      /// ### ### ### ###
      ///  MOUSE EVENTS
      ///
      ///
      /// ### ### ### ###


      void main_application::button_pressed(const bleunw::yaggler::events::mouse_status &ms, bleunw::yaggler::events::mouse_buttons::mouse_buttons mb)
      {
        (void)ms;
        (void)mb;
      }

      void main_application::button_released(const bleunw::yaggler::events::mouse_status &ms, bleunw::yaggler::events::mouse_buttons::mouse_buttons mb)
      {
        (void)ms;
        (void)mb;
      }

      void main_application::mouse_moved(const bleunw::yaggler::events::mouse_status &ms)
      {
        (void)ms;
      }



      /// ### ### ### ###
      /// KEYBOARD EVENTS
      ///
      ///
      /// ### ### ### ###


      void main_application::key_pressed(const bleunw::yaggler::events::keyboard_status &ks, bleunw::yaggler::events::key_code::key_code kc)
      {
        if (ks.modifiers == bleunw::yaggler::events::modifier_keys::none && kc == bleunw::yaggler::events::key_code::escape)
          do_quit = true;
      }

      void main_application::key_released(const bleunw::yaggler::events::keyboard_status &ks, bleunw::yaggler::events::key_code::key_code kc)
      {
        (void)ks;
        (void)kc;
      }
    } // namespace sample
  } // namespace klmb
} // namespace neam

#endif /*__N_8334687671285212906_717271234__APP_EVENTS_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

