
// test klmb fragment shader
// to see if yaggler works ;)

// Specify which version of GLSL we are using.

#version 330 core

#line 10

// #ifndef USING_KLMB_FRAMEWORK
// # error This shader must be used with the K:LMB framework
// #endif

#ifdef USING_KLMB_FRAMEWORK

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0);

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

// some defines controlling what's going on:
#define SHOW_TRIANGLES
#define FAST_LIGHTNING
#define SIMPLE_REFLECT

// in vars
in vec3 tri_distance;
in vec3 t_position;
in vec3 gbuffer_oriented_normal;
in vec4 gbuffer_vertex_position;
in vec4 gbuffer_vertex_orig_position;
in vec4 gbuffer_normal;

// uniforms
uniform float global_time;
uniform sampler2D s_texture;
uniform vec2 grid_size;
uniform float wireframe = 0.;

uniform vec3 camera_position;

float amplify(float d, float scale, float offset)
{
  d = scale * d + offset;
  d = clamp(d, 0, 1);
  d = 1 - exp2(-2*d*d);
  return d;
}

// sample the distance field
float mapDF(vec3 pos)
{
//  return length(pos - vec3(0., 0., 0.)) - 5.;
  pos.y += 25.;
  vec3 p = vec3(pos.x * sin(global_time) - pos.y * cos(global_time), pos.x * cos(global_time) + pos.y * sin(global_time), pos.z);
  vec2 q = vec2(length(p.xz) - 5.5, p.y);
  return length(q) - 2.5;
}

// return the color
vec3 raymarch(vec3 pos, vec3 dir)
{
  float d = 0;
  float acc = 0;
  vec3 color = vec3(0.);
  for (int i = 0; i < 120; ++i)
  {
    d = mapDF(pos + acc * dir);
    acc += (d);
    color += (5. - clamp(d / 0.5, 0., 5.)) / 5.;
    if (abs(d) < .001)
      color += (0.75);
      return color;
  }
//   if (length(color) < 0.1)
//     discard;
  return color;
}

void KLMB_MAIN_FUNCTION()
{
/*  vec3 color = vec3(0.2, 0.2, 0.2);

  vec2 uv = 1. - ((floor(gbuffer_vertex_orig_position.xz) + grid_size / 2.) / grid_size);
  vec4 tcolor = texture(s_texture, uv);
  float color_strength = length(tcolor);

#ifdef SHOW_TRIANGLES
  float d1 = 0.;
  if (wireframe > 0.)
  {
    d1 = min(min(tri_distance.x, tri_distance.y), tri_distance.z);
    d1 = clamp(1. - clamp(amplify(d1, 8., 0.0), 0., 100.) * 1.3, 0., 1.) * wireframe;
  }
#else
  const float d1 = 0.;
#endif

#ifdef FAST_LIGHTNING
  float l_factor = max(gbuffer_oriented_normal.y + length(gbuffer_oriented_normal.xz) / 4., 0.);
//   l_factor = clamp(pow(l_factor, 4.0), 0., 1.);
#else
  const float l_factor = 1.;
#endif

#ifdef SIMPLE_REFLECT
  vec3 ref_color = vec3(0., 0., 0.);
  {
    vec3 pos = gbuffer_vertex_position.xyz;
    vec3 dir = reflect(-normalize(gbuffer_vertex_position.xyz), gbuffer_oriented_normal);

    
  }
#else
  const vec3 ref_color = vec3(0., 0., 0.);
#endif

  KLMB_SHARED_NAME(color_0) = l_factor * (vec4(tcolor.xyz + ref_color, 1.0) - d1 * vec4(0.20, 0.20, 0.20, 0.0));
  */
//   KLMB_SHARED_NAME(color_0) = vec4(gbuffer_oriented_normal.x * 0.5 + 0.5,
// //                                    abs(gbuffer_vertex_orig_position.y / 30.),
//                                    gbuffer_oriented_normal.y * 0.5 + 0.5,
//                                    gbuffer_oriented_normal.z * 0.5 + 0.5,
//                                    1.);
//   KLMB_SHARED_NAME(color_0) = vec4(abs(gbuffer_vertex_orig_position.x / 30.),
//                                     abs(gbuffer_vertex_orig_position.y / 30.),
//                                     abs(gbuffer_vertex_orig_position.z / 30.),
//                                    1.);

  KLMB_SHARED_NAME(color_0).a = 1.;
  vec3 dir = normalize(gbuffer_vertex_position.xyz - camera_position);
    //KLMB_SHARED_NAME(color_0).rgb = (camera_position  / 100.);return;
  KLMB_SHARED_NAME(color_0).rgb = raymarch(gbuffer_vertex_orig_position.xyz, -dir);

  float d1 = 0.;
  if (wireframe > 0.)
  {
    d1 = min(min(tri_distance.x, tri_distance.y), tri_distance.z);
    d1 = clamp(1. - clamp(amplify(d1, 8., 0.0), 0., 100.) * 1.3, 0., 1.) * wireframe;
  }
  d1 = clamp(d1, 0.0, 1.);
  KLMB_SHARED_NAME(color_0).r += step(0.5, KLMB_SHARED_NAME(color_0).r) * -d1 + step(KLMB_SHARED_NAME(color_0).r, 0.5) * d1;
}

#else


// output something... _*red*_ :)
void main()
{
  color_0 = vec4(1., .0, .0, 1.);
}

#endif
