//
// file : blur.hpp
// in : file:///home/tim/projects/yaggler/tools/font-generator/blur.hpp
//
// created by : Timothée Feuillet on linux-coincoin.tim
// date: 02/04/2014 19:29:18
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

#ifndef __N_21004175581958645965_640948520__BLUR_HPP__
# define __N_21004175581958645965_640948520__BLUR_HPP__

#include <tools/ct_string.hpp>

// uniforms:
// - sampler2D fnt_texture;
// - vec2 buffer_size;
// - vec2 direction;
// - float initial; (default: 0.1)
//
// defines:
// - NUM_BLUR_SAMPLES (default: 4)


// pack the shader in the executable
constexpr neam::string_t blur_shader_string = R"(
#version 330 core
#line 44

#define KLMB_IS_USING_FRAMEWORK
#define KLMB_IS_ENTRY_POINT
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

#define NUM_BLUR_SAMPLES 4.
#define SQUARE(x)       ((x) * (x))
#define GAUSS_DISTRIB   (sqrt(-SQUARE(NUM_BLUR_SAMPLES + 1.0f) / -11.082527f))
#define GAUSS(x)        (1.0 / (2.506628275 * GAUSS_DISTRIB) * exp(-(SQUARE(x) / (2.0 * SQUARE(GAUSS_DISTRIB)))))

uniform sampler2D fnt_texture;
uniform vec2 buffer_size;
uniform vec2 direction;
uniform float initial = 0.1;
in vec3 vertex_position;

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0);

void KLMB_MAIN_FUNCTION()
{
  vec2 o = normalize(direction) / buffer_size;
  vec2 uv = vertex_position.xy * 0.5 + 0.5;

  float denom = initial;
  vec4 sum = texture(fnt_texture, uv) * initial;

  for (float i = 0.; i < NUM_BLUR_SAMPLES; ++i)
  {
    float idx = i - (NUM_BLUR_SAMPLES * 0.5);
    float c = GAUSS(idx);
    sum += texture(fnt_texture, uv + o * idx) * c;
    denom += c;
  }
  KLMB_SHARED_NAME(color_0) = vec4((sum / denom).xyz, 1.);
})";

#endif /*__N_21004175581958645965_640948520__BLUR_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 

