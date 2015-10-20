

// !<:LMB fragment shader

#version 330

// // raymarcher defines
#define RM_MAX_STEPS                    550.


// // cam defines
#define ROTATION_SPEED                  0.10

#define CAM_DISTANCE                    5.
#define CAM_DISTANCE_MVT                0.


// // material define
#define MAT_REFRACT
// #define MAT_REFLECT

// at each bounce, the factor of the reflection / refraction
#define MAT_COLOR_ABSORPTION            0.99

// glow
#define GLOW_MAX_DST                    6.
#define GLOW_COLOR                      vec3(0.7, 0.75, 1.5)
#define GLOW_STRENGTH                   0.0002


// // fractal defines
#define FRAC_STEPS                      4

// #define FRAC_XY_SYMMETRY
#define FRAC_SUBSTRACT
#define FRAC_NO_PLANE

// // sanity
#if defined MAT_REFLECT && defined MAT_REFRACT
#error you can't relfect and refract at the same time
#endif

uniform float global_time;
uniform vec2 screen_resolution;

uniform sampler2D texture;

in vec4  vertex_position;


out vec4 color_1;

// that base element
float base_elem(vec3 p, vec3 size)
{
  vec3 b = abs(p - vec3(0.0, 2.0, 0)) - size;
  float d = min(max(b.x, max(b.y, b.z)), 0.0) + length(max(b, 0.0));

  return d;
}

// where the computations (the distance field) are
float map(vec3 position)
{
  // root;
  float d = 10000000.; // uh... :D

  // size
  float c = 1.0;
  position.xz = mod(position.xz + 8. * 0.5, 8.) - 8. * 0.5;
  vec3 p = position;


  for (int i = 0; i < FRAC_STEPS; ++i)
  {
    // X/Y/Z symetry
    p.xz = abs(p.xz);
    p.y = p.y;

#ifdef FRAC_XY_SYMMETRY
    // 45deg symmetry
    p = abs(p);
    p.x = min(p.y, p.x);
    p.y = max(p.y, abs(position.x));
#endif

    d = min((base_elem(p / c, vec3(0.25 / c, 2.0, 1.0)) * c), d);

    p.z -= c * 2.;
    c *= 0.3333;
  }

#ifdef FRAC_SUBSTRACT
  d = max(-d, base_elem(position + vec3(0, -1, 0), vec3(0.20, 3.1, 3.)));
#endif

#ifdef FRAC_NO_PLANE
  return d;
#else
  return min(d, position.y);
#endif
}

// from a nice shader: https://www.shadertoy.com/view/lsXGWl
float ambiant_occlusion(const vec3 pos, const vec3 n)
{
  float aodet = 0.00009 * 40.;
  float ao = 0.0;
  float sca = 3.0;
  for(int i = 0; i < 5; i++)
  {
    float hr = aodet * float(i * i);
    vec3 aopos = n * hr + pos;
    float dd = map(aopos);
    ao += -(dd-hr) * sca;
    sca *= 0.7;
  }
  return clamp(1.0 - 5.0 * ao, 0., 1.0);
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
vec3 raymarch(vec3 from, in vec3 dir)
{
  vec3 color = vec3(0, 0, 0);

  float dst_acc = 0.;
  float total_dst = 0.;
  float dst_rmg = 0.;
  vec3 pos;

  bool into = false;
  float ref = 1.;

  for(int it = 0; it < RM_MAX_STEPS && ref > 0.001; it += 1)
  {
    pos = from + dir * dst_acc;
    dst_rmg = map(pos) / 2.;
    dst_acc += dst_rmg;
    total_dst += dst_rmg;

    if ((dst_rmg) <= 0.001 && !into) // close enought
    {
      vec3 n = normal(pos);
      color += (vec3(0.9, 0.9, 0.9) / 2. * ambiant_occlusion(pos, n)) * ref /*/ (total_dst * total_dst / 20. + 1.)*/ / 5;

      from = pos;
#ifdef MAT_REFRACT
      into = true;
      dir = refract(dir, n, 1.03);
#else if defined MAT_REFLECT
      dir = reflect(dir, n);
#endif
      dst_acc = 0.0051;

      ref *= MAT_COLOR_ABSORPTION;
#if !defined MAT_REFLECT && !defined MAT_REFRACT
      break;
#endif
    }
#ifdef MAT_REFRACT
    else if ((dst_rmg) <= 0.001 && into)
    {
      color += (vec3(0.2, 0.3, 0.5) / 2.) * ref /*/ (total_dst * total_dst / 20. + 1.)*/ / 250;
      from = pos;
      dst_acc = 0.01;

      ref *= MAT_COLOR_ABSORPTION;
    }
    else if ((dst_rmg) >= 0.001 * 2. && into)
    {
//       color += (vec3(0.9, 0.9, 0.9) / 2.) * ref;
      from = pos;
      dst_acc = 0.1;
      vec3 n = normal(pos);
      dir = refract(dir, -n, 1.03);
      into = false;
    }
#endif
    else if (dst_rmg < GLOW_MAX_DST && !into && ref >= 0.1)
      color += GLOW_STRENGTH * GLOW_COLOR * (GLOW_MAX_DST - dst_rmg)/ ((dst_acc + 1.) / 10.) * ref * ref; // glow
  }
  return color;
}

// create a simple rotation matrix on Y axis
mat3 y_rotation_matrix(float angle)
{
  vec3 axis = vec3(0, 1, 0);
  float s = sin(angle);
  float c = cos(angle);
  float oc = 1.0 - c;
  return mat3
  (
    c,          0.0,                            axis.y * s,
    0.0,        oc * axis.y * axis.y + c,       0.0,
    -axis.y * s, 0.0,                           c
  );
}

void main()
{
  vec2 frag_coord = (vertex_position.xy / 2 + 0.5) * screen_resolution;

  vec2 uv = (vertex_position.xy); // some good trick for a fs quad :)
  vec3 dir = normalize(vec3(uv * 1.0, 1.0));

  mat3 rotmat = y_rotation_matrix(mod(global_time * ROTATION_SPEED, 2 * 3.1415927));

  vec3 pos = vec3(0., 0.5, -1. * CAM_DISTANCE + sin(global_time) * CAM_DISTANCE_MVT);
  color_1 = vec4(raymarch(pos * rotmat, dir * rotmat), 1.0);

}


