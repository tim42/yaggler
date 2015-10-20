
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
uniform float light;

// input [from vertex stage]
in vec3 orig_vertex_position;


// from http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
// maybe it's more a HSL -> RGB
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void KLMB_MAIN_FUNCTION()
{
  vec2 diff_uv = end_uv - start_uv;
  vec2 uv = vec2(orig_vertex_position.x, 1 - orig_vertex_position.y) * diff_uv + start_uv;

  KLMB_SHARED_NAME(color_0).xyz = hsv2rgb(vec3(uv, light));
  KLMB_SHARED_NAME(color_0).a = alpha_factor;
}

