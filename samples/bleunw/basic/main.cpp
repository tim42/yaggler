//
// Copyright (C) 2013-2014 Timothée Feuillet
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


#include <setup.hpp>

using opengl_version = neam::yaggler::setup::opengl<3, 3, neam::yaggler::setup::opengl_profile::core/*, neam::yaggler::setup::opengl_context_flag::debug*/>;

// disable messages
// #define YAGGLER_NO_MESSAGES

#include <yaggler.hpp>

// lets include klmb main header.
#include <klmb/klmb.hpp>

// lets include some bleunw files
#include <bleunw/events.hpp>

#include <iostream>
#include <iomanip>

#include "app.hpp"

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  neam::cr::out.log_level = neam::cr::stream_logger::verbosity_level::debug;

  // init yaggler (+glfw)
  neam::yaggler::yaggler_init yi;

  neam::klmb::sample::main_application app;
  app.run();

  return 0;
}
