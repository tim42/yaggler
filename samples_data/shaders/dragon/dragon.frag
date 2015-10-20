

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


float amplify(float d, float scale, float offset)
{
  d = scale * d + offset;
  d = clamp(d, 0., 1.);
  d = 1. - exp2(-2. * d * d);
  return d;
}

void KLMB_MAIN_FUNCTION()
{
  float d1 = min(min(tri_distance.x, tri_distance.y), tri_distance.z);
  d1 = clamp(amplify(d1, 8., 0.0), 0., 100.) * 1.3;

  const float dst = 10.;
  if (gbuffer_position.z > dst)
  {
    float dt = clamp(dst + 5. - gbuffer_position.z, 0., 5.) / 5.;
    d1 = dt * d1 + 1. - dt;
  }

  KLMB_SHARED_NAME(color_0).rgb = /*d1*/ /** (1 - gbuffer_position.z / 25.5)*/  (vec3(0.9, 0.9, 1.0)) * (dot(gbuffer_normal.xy,gbuffer_normal.xy) * 1.2) / 1.5;
//   KLMB_SHARED_NAME(color_0).rgb = vec3(0, abs(cos(global_time)), abs(sin(global_time * 2.)));
}

#else


// output something... _*red*_ :)
void main()
{
  color_0 = vec4(1., .0, .0, .0);
}

#endif
