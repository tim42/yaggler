//
// file : yaggler.hpp
// in : file:///home/tim/projects/yaggler/yaggler/yaggler.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 08/10/2013 17:05:49
//
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


#ifndef __YAGGLER_HPP__
# define __YAGGLER_HPP__

// yaggler/setup.hpp hasn't been included before (and ::opengl_version isn't setted...)
#ifndef __N__SETUP_HPP__
#error please include yaggler/setup.hpp before yaggler/yaggler.hpp and define the ::opengl_version typedef

// #include <yaggler/setup.hpp>
// using opengl_version = neam::yaggler::setup::opengl<3, 1>;
//
// #include <yaggler/yaggler.hpp>

#error (see the comment above)
#endif

// some defines/switches

// disable the print of the init message, and png loading messages
// #define YAGGLER_NO_MESSAGES

// avoid some checks at ct. (notably in the texture code)
// #define YAGGLER_NO_FUCKING_TESTS


// GL, glew and other external things (see the third_party folder)
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

// init and window
#include "yaggler_except.hpp"
#include "yaggler_init.hpp"
#include "yaggler_glfw_window.hpp"

// types
#include "yaggler_type.hpp"

// tools
#include "tools/ct_string.hpp"
#include "tools/string.hpp"
#include "tools/embed.hpp"
#include "tools/ref.hpp"
#include "tools/chrono.hpp"
#include "tools/tuple.hpp"
#include "tools/array_wrapper.hpp"

// shader stuff
#include "shader/shader.hpp"

// texture stuff
#include "texture/texture.hpp"

// geom stuff
#include "geometry/geometry.hpp"

// simple check stuff
#if defined __NO_INLINE__ or (not defined __OPTIMIZE__ and defined __GNUC__)
#warning you shall enable optimisations / do not disable inlining to get some perfs from yaggler.
#endif

namespace neam
{
  namespace yaggler
  {
  } // namespace yaggler
} // namespace neam

#endif /*__YAGGLER_HPP__*/
