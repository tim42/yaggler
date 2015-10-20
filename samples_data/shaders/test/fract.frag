
// !<:LMB fragment shader
// fake volume-like renderer

#version 330

#line 6

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT
// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

// raymarcher defines
#define RM_MAX_STEPS                    100.
#define INTO_STEP_DST                   .0035

#define FRACT_DEPTH                     3
/*#define FRACT_DEPTH                     sin(global_time) * 4 + 3*/

// prog defines
#define ROTATION_SPEED                  0.2000

#define CAM_DISTANCE                    40.
#define CAM_DISTANCE_MVT                00.

// glow
#define GLOW_MAX_DST                    2.0
#define GLOW_COLOR                      vec3(0.7, 0.70, 1.5)
#define GLOW_STRENGTH                   0.05


uniform float global_time;
uniform vec2 screen_resolution;

in vec3 vertex_position;


KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0); // color                (rgba)


mat3 rotation_matrix_y(float angle)
{
  float s = sin(angle);
  float c = cos(angle);
  return mat3
  (
    c,          0.0,                           s,
    0.0,        1.0,                           0.0,
    -s,         0.0,                           c
  );
}

// where the computations (the distance field) are
float map(vec3 position)
{
  /*const float c = 15.;

  vec3 p = mod(position, 50.) - 50. * 0.5;

  float d = length(p) - c;

  float td;

  p = mod(position, c) - c * 0.5;
  td = length(p) - c / 1.8;
  return max(-td, d);*/

  float c = 15.;

  float d = length(max(abs(position) - c, 0.0));

  for (int i = 0; i < FRACT_DEPTH; ++i)
  {
    d = max(-(length(mod(position + c * 0.5, c) - c * 0.5) - c * 0.4), d);
//     d = max(-(length(mod(position / 2/*+ kc * 0.5*/, kc) - kc * 0.5) - kc / 2.0), d);
    c *= 0.5;// * (cos(global_time * 0.1) * 0.5 + 1.0);
  }

  return d;
}

// do the raymarch
vec3 raymarch(vec3 from, in vec3 dir)
{
  vec3 color = vec3(0, 0, 0);

  float dst_acc = 0.;
  float dst_rmg = 0.5;

  for(int it = 0; it < RM_MAX_STEPS && dst_rmg > 0.01; ++it)
  {
    dst_rmg = map(from + dir * dst_acc) * 0.50;
    dst_acc += max(INTO_STEP_DST, (dst_rmg));

//       color = vec3(0, 0, 1);
      color += GLOW_STRENGTH * GLOW_COLOR * max(0., GLOW_MAX_DST - /*abs*/(dst_rmg)) * 10. / ((dst_acc + 1.)); // "glow"
  }
  if (dst_rmg <= 0.01)
    color += vec3(0.01, 0.01, 0.2) * 2.;

  return sqrt(color);
}

#define PI      3.1415927
void KLMB_MAIN_FUNCTION()
{
  vec2 uv = (vertex_position.xy); // some good trick for a fs quad :)
  uv.x *= screen_resolution.x / screen_resolution.y;
  vec3 dir = normalize(vec3(uv * 1.0, 1.0));

  mat3 rotmat = rotation_matrix_y(mod(global_time * ROTATION_SPEED, 2. * PI));

  vec3 pos = vec3(0., 5., -1. * CAM_DISTANCE + sin(global_time * ROTATION_SPEED) * CAM_DISTANCE_MVT);
  KLMB_SHARED_NAME(color_0) = vec4(raymarch(pos * rotmat, dir * rotmat), 1.0);
}


