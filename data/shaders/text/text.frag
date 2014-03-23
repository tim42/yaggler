

// test klmb fragment shader
// to see if yaggler works ;)

// Specify which version of GLSL we are using.

#version 330 core

#line 10

// #ifndef USING_KLMB_FRAMEWORK
// # error This shader must be used with the K:LMB framework
// #endif


#ifdef USING_KLMB_FRAMEWORK

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT
// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0);

// from the geom stage
in vec4 primitive_count;
in vec2 uv;

uniform float global_time;
uniform vec2 screen_resolution;

uniform float char_count;
uniform sampler2D font_texture;


void KLMB_MAIN_FUNCTION()
{
  float mask = texture(font_texture, uv).r;
  vec4 color = vec4(1., 1., 1., 1.);

  color.a = smoothstep(0.439, 0.47, mask);

  if (color.a < 0.1)
    discard;

  KLMB_SHARED_NAME(color_0) = vec4(color.rgba);//(vec3(primitive_count.x / 7., abs(cos(global_time)), 1. - primitive_count.x / 7));
}

#else


// output something... _*red*_ :)
void main()
{
  color_0 = vec4(1., .0, .0, .0);
}

#endif
