
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

// output
KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0);

// uniforms
uniform vec2 start_uv;
uniform vec2 end_uv;

uniform float alpha_factor;

uniform vec3 color = vec3(1., 1., 1.);

// input [from vertex stage]
in vec3 orig_vertex_position;

void KLMB_MAIN_FUNCTION()
{
  vec2 diff_uv = end_uv - start_uv;
  KLMB_SHARED_NAME(color_0).xyz = color;
  KLMB_SHARED_NAME(color_0).a = alpha_factor;
}

