

// test klmb fragment shader
// to see if yaggler works ;)

// Specify which version of GLSL we are using.

#version 330 core

#line 10

// #ifndef USING_KLMB_FRAMEWORK
// # error This shader must be used with the K:LMB framework
// #endif

uniform float global_time;
uniform vec2 screen_resolution;
uniform vec3 material_color;

uniform mat4 vp_matrix;
uniform mat4 view_matrix;
uniform mat4 object_matrix;

in vec4  gbuffer_position;

#ifdef USING_KLMB_FRAMEWORK

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0);

in vec3 gbuffer_normal;
in vec3 tri_distance;


// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

void KLMB_MAIN_FUNCTION()
{
  vec3 oriented_normal = normalize(vec4(gbuffer_normal.zyx, 0.) * (-view_matrix) * inverse(-object_matrix) ).xyz;
  KLMB_SHARED_NAME(color_0).rgb = material_color * exp(length(oriented_normal.xzy * 10.));
}

#else


// output something... _*red*_ :)
void main()
{
  color_0 = vec4(1., .0, .0, .0);
}

#endif
