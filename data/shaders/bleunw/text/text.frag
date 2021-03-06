
// Specify which version of GLSL we are using.
#version 330 core

#line 5

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT
// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0);

// from the geom stage
in vec2 uv;
in vec3 font_scale;

uniform float global_time;
uniform vec2 screen_resolution;

uniform float char_count;
uniform sampler2D font_texture;
uniform vec4 font_color;


void KLMB_MAIN_FUNCTION()
{
  float mask = texture(font_texture, uv).r;
  vec4 color = font_color;
  // take account of the scale of the font.
  color.a = smoothstep(0.400 + clamp(length(font_scale) * 0.1, 0., 0.055), 0.47, mask) * font_color.a;

  if (color.a < 0.05)
    discard;

  KLMB_SHARED_NAME(color_0) = color.rgba;
}
