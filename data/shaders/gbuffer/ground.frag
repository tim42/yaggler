
#version 330 core

#line 4

// raytraced ground plane + fog

#ifdef USING_KLMB_FRAMEWORK

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            2

uniform sampler2D lit_scene;            // color                (rgba)
uniform sampler2D lit_geometry;         // normal + depth       (rgb + a)
uniform sampler2D scene;                // color                (rgba)
uniform sampler2D geometry;             // color                (rgba)

uniform mat4 cam_view;
uniform vec3 cam_pos;

uniform vec3 ambiant_color = vec3(0.2, 0.2, 0.2);

uniform float global_time;
uniform vec2 buffer_size;               // screen size

uniform float selection_factor = 1.;

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0); // color          (rgba)
KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_1); // normal + depth (rgb + a)

struct hit_point
{
  vec3 normal;
  float depth;

  vec4 color;
};

//
// WARNING: FULL OF MAGIC NUMBERS THAT ARE HERE WITHOUT ANY REASON... :(
//

#define PI 3.14159265358979323846264338

// this func raytrace... a plane :D
hit_point raytrace(vec3 from, vec3 dir, vec4 geom, vec4 color)
{
  hit_point hp;
  hp.color = color;
  hp.normal = geom.xyz;
  hp.depth = geom.w; // no hits

  float dst_acc = geom.w;

  vec3 ppos = vec3(0., -1.0, 0.);

  if (from.y >= ppos.y && dir.y <= -0.)
  {
    float k = -((from.y - ppos.y) / dir.y);
    vec3 pos = from + (dir) * k;

    // why 2.30 ??? WHY ? :'(
    k = (k) / 2.30;


    const float line_width = 0.05;

    if (geom.w == 0. || (k) < geom.w - line_width) // not hidden by any geometry
    {
      dst_acc = (k);

      float ch_color = clamp(abs(mod(pos.x, 2.) - 1.0) * 4., 0., 0.4) * clamp(abs(mod(pos.z, 2.) - 1.0) * 4., 0., 0.4);

      // show red zones (hidden geom)
      if (geom.w == 0 || ch_color < 0.1)
        hp.depth = dst_acc;
      if (ch_color < 0.1 && k < 110)// 0.1
      {
        hp.color = vec4(0.20, 0.20, 0.20, selection_factor);
        hp.normal = (vec3(0., 1., 0.));
      }
      else
        dst_acc = geom.w;
      if ((k) < geom.w)
      {
        hp.color.rgb = abs(sqrt((hp.color.rgb / 2. * color.rgb / 10.) * vec3(10.75, 1., 1.)));
        hp.color.a = color.a * 0.85;
        if (ch_color < 0.1)
          hp.depth = geom.w;
      }
    }
    else if (geom.w != 0. && k > geom.w - line_width && k < geom.w + line_width) // red line
      hp.color = vec4(1., 0., 0., 1.); // always red

  }
  if (dst_acc == 0.)
  {
    dst_acc = 100.;
    hp.color.rgb += (ambiant_color * clamp(dst_acc / 25., 1., 10.)) / 7.;
    hp.color.a = selection_factor;
  }
//   else
//     hp.color.rgb += (vec3(0.3, 0.5, 0.9) * (clamp(dst_acc / 25., 2., 30.) - 2.) / 4.);

//   hp.color.rg /= (exp(dst_acc / 15. - 20.) ) + 1;
//   hp.color.b /= exp(dst_acc / 25. - 10.) + 1;

  return hp;
}

void KLMB_MAIN_FUNCTION()
{
  vec2 uv = (gl_FragCoord.xy) / buffer_size;

//   uv *= (1 - cos(uv * PI) / 5.);
//   uv.y = 1 - uv.y;
//   uv /= 2.;

  // texture fetch
  vec4 scene_color = texture(scene, uv);
  vec4 ltscene_color = texture(lit_scene, uv);
  vec4 geom = texture(geometry, uv);
  vec4 ltgeom = texture(lit_geometry, uv);

  if (ltgeom.w > 0)
  {
    geom = ltgeom;
    scene_color.a = (ltscene_color.a);
  }

  scene_color.xyz = ltscene_color.xyz + ambiant_color * scene_color.xyz;

  // raytrace the plane
  hit_point hp;
  hp.color = vec4(0, 0, 0, 0);
  hp.normal = vec3(0, 0, 0);
  hp.depth = 0;
  hit_point t_hp; // tmp
  hit_point t2_hp; // tmp
  float acum = 0;

  // antialiase the grid
  for (int i = 0; i < 2; ++i)
  {
    for (int j = 0; j < 2; ++j)
    {
      // compute correct uv
      uv = 2. * (gl_FragCoord.xy + vec2(i - 0.5, j - 0.5) * .2) / buffer_size - 1.;
      float w = tan((PI / 4.) / 2. );
      uv.x *= buffer_size.x / buffer_size.y;

      // compute direction vector
      vec3 dir = normalize((vec4(uv, -1.0 / w, 0.)).xyz);

      // distance with the plane in z = w
      // (get correct distance)
      {
        float k = ((-w) / dir.z);
        dir *= k;
      }
      dir = (vec4(dir, 0) * cam_view).xyz;
//       t_hp = raymarch(cam_pos.xyz, dir, geom, scene_color);
//       t_hp = raytrace(cam_pos.xyz, dir, vec4(t_hp.normal, t_hp.depth), t_hp.color);
      t_hp = raytrace(cam_pos.xyz, dir, geom, scene_color);

      float coef = 1 - (abs(float(i) - 1) + abs(float(j) - 1)) / 2.;
      hp.color += t_hp.color * coef;
      hp.normal += t_hp.normal * coef;
      hp.depth += t_hp.depth * coef;
      acum += coef;
    }
  }

  // result:
  uv += cos(length(uv)); // nice color effect
  uv = (abs((1. - uv) / 5.));
  KLMB_SHARED_NAME(color_0) = hp.color / acum;
//   KLMB_SHARED_NAME(color_0).a = 1.;

  KLMB_SHARED_NAME(color_1) = vec4(hp.normal / acum, hp.depth / acum); // for latter passes
}

/// ### ### ### ##
/// ### ### ### ##
/// ### ### ### ##

// raymarcher defines
#define RM_MAX_STEPS                    170.
// #define RM_MAX_STEPS                    40.
#define INTO_STEP_DST                   0.35

// prog defines
#define ROTATION_SPEED                  0.2000

#define CAM_DISTANCE                    30.
#define CAM_DISTANCE_MVT                10.

// glow
#define GLOW_MAX_DST                    2.0
#define GLOW_COLOR                      vec3(0.7, 0.70, 0.7)
#define GLOW_STRENGTH                   0.05
// where the computations (the distance field) are
float map(vec3 position)
{
  float c = 4.;

  position -= vec3(10, 2, 0);
//   position = mod(position - 40, 80) - 80 * 0.5;
  float d = length(max(abs(position) - c, 0.0));

  for (int i = 0; i < 1; ++i)
  {
    d = max(-(length(mod(position + c * 0.5, c) - c * 0.5) - c * 0.4), d);
//     d = max(-(length(mod(position / 2/*+ kc * 0.5*/, kc) - kc * 0.5) - kc / 2.0), d);
    c *= 0.5;// * (cos(global_time * 0.1) * 0.5 + 1.0);
  }
  return d;
//   float td;

//   td = length(p) - c / 1.8;
//   return max(-td, d);
}
// compute the normal
// (from a nice shader: https://www.shadertoy.com/view/lsXGWl)
vec3 normal(in vec3 pos)
{
  vec2 eps = vec2(0.001, 0.0);
  vec3 nor = vec3(
      map(pos + eps.xyy) - map(pos - eps.xyy),
      map(pos + eps.yxy) - map(pos - eps.yxy),
      map(pos + eps.yyx) - map(pos - eps.yyx));
  return normalize(nor);
}
// do the raymarch
hit_point raymarch(vec3 from, in vec3 dir, vec4 geom, vec4 color)
{
  hit_point hp;
  hp.color = color;
  hp.normal = geom.xyz;
  hp.depth = geom.w;


  vec3 color_acc = vec3(0, 0, 0);
  float dst_acc = 0.;
  float dst_rmg = 0.;
  bool hit = false;

  for(int it = 0; it < RM_MAX_STEPS && (dst_acc / 2.3 < geom.w || geom.w == 0); ++it)
  {
    dst_rmg = map(from + dir * dst_acc);
    dst_acc += ((dst_rmg));

    if ((dst_rmg) <= 0.01)
    {
      hp.color.xyz = sqrt(vec3(1, 1, 1) * 1 /*- color_acc * 0.25*/);
      hp.depth = dst_acc / 2.3;
      hp.normal = reflect(dir, normal(from + dir * dst_acc));
      hit = true;
      break;
    }
    else if (/*abs*/(dst_rmg) <= GLOW_MAX_DST)
      color_acc += GLOW_STRENGTH * GLOW_COLOR * (GLOW_MAX_DST - /*abs*/(dst_rmg)) * 10. / ((dst_acc + 1.)); // "glow"
  }
//   if (!hit)
//     hp.color.xyz += sqrt(color_acc);

  return hp;
}

/// ### ### ### ##
/// ### ### ### ##
/// ### ### ### ##
#endif
