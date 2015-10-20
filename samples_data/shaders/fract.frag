

// !<:LMB fragment shader

#version 330

// raymarcher defines
#define RM_MAX_STEPS                    1000.

// prog defines
#define ROTATION_SPEED                  0.5000

#define CAM_DISTANCE                    30.
#define CAM_DISTANCE_MVT                10.

// at each bounce, the factor of the reflection / refraction
#define MAT_COLOR_ABSORPTION            0.99999

// glow
#define GLOW_MAX_DST                    10.
#define GLOW_COLOR                      vec3(0.7, 0.75, 1.5)
#define GLOW_STRENGTH                   0.0000002


uniform float global_time;
uniform vec2 screen_resolution;

uniform sampler2D texture;

in vec4  vertex_position;


out vec4 color_1;

mat3 rotationMatrixY(float angle)
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


// where the computations (the distance field) are
float map(vec3 position)
{
//   vec2 c = vec2(3, 3);
//   position = mod(position, c) - 0.5 * c;

//   mat3 mat = rotationMatrixY(3.1415927 / 3.);

//   for (int i = 0; i < 1; i++)
//   {
    // rounded rectangle equation:
//     p.xz = mod(p.xz, c) - 0.5 * c;
//     p = 1. / (abs(p) + 1.);
//     p.y -= 2;
//     p.yxz *= mat;
//     p.xzy *= mat;
//     c /= 2;
//   }
//     d = min(length(max(abs(p.xyz) - vec3(0.1, 1.0, 0.1), vec3(0.0))) - 0.09, d);
//   p.zy = mod(p.yz, 30.) - 30. * 0.5;

  vec3 p;
  float c = 15.;
  p = position;
  float d = length(p) - c;
  vec3 b;
//   float d = length(max(abs(p) - c, 0.0));
//   b = abs(p) - c + 1 ;
//   float d = min(max(b.x, max(b.y, b.z)), 0.0) + length(max(b, 0.0));
//   c /= 4.;

  float td;

  float sgn = -1;

  for (int i = 0; i < 1; ++i)
  {
    p = position;
    p = mod(p + c * 0.5, c) - c * 0.5;
    td = length(p) - c / 2.1;
//     b = abs(p) - c / 3.3;
//     td = min(max(b.x, max(b.y, b.z)), 0.0) + length(max(b, 0.0));
    d = max(-td/* * sgn*/, d);
    c *= 0.5;
    sgn *= -1;
//     c *= 0.75 * abs(sin(global_time / 5.) + 0.1);
//     c = sqrt(c);
  }

//   d = length(tp.zy) - 10. /*abs(sin(tp.x) + sin(p.y)) * 1.*/;


  return d;

//   float r = 4;
// 
// 
//   mat3 rot_mat = rotationMatrixY(3.1415927 / 6.);
// 
//   for (int i = 0; i < 20; ++i)
//   {
//     d = (length(max(abs(position.yz) - vec2(0, 0), 0.)) - d);
//     position = position.zxy;
//     position.y += 5;
//     position *= rot_mat;
//     r *= 0.8;
//   }
// 
//   return d;
// 
// 
//   // others
//   return length(max(abs(position) - vec3(0, 0, 0), 1.0)) - 2.;
//   return length(position) - 2.;
}

// compute the normal
vec3 normal(in vec3 pos)
{
  vec2 eps = vec2(0.001, 0.0);
  vec3 nor = vec3(
      map(pos + eps.xyy) - map(pos - eps.xyy),
      map(pos + eps.yxy) - map(pos - eps.yxy),
      map(pos + eps.yyx) - map(pos - eps.yyx));
  return normalize(nor);
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
      into = true;
//       dir = refract(dir, n, 1.03);
//       dir = reflect(dir, n);
      dst_acc = 0.0051;

      ref *= MAT_COLOR_ABSORPTION;
    }
    else if ((dst_rmg) <= 0.001 && into)
    {
      color += (vec3(0.5, 0.5, 0.5) / 2.) * ref /*/ (total_dst * total_dst / 20. + 1.)*/ / 50;
      from = pos;
      dst_acc = 0.1;

      ref *= MAT_COLOR_ABSORPTION;
    }
    else if ((dst_rmg) >= 0.001 * 2. && into)
    {
//       color += (vec3(0.9, 0.9, 0.9) / 2.0) * ref;
      color *= 0.25;
      from = pos;
      dst_acc = 0.1;
//       vec3 n = normal(pos);
//       dir = refract(dir, -n, 1.03);
      into = false;
    }

    else if (dst_rmg < GLOW_MAX_DST && !into && ref >= 0.3)
      color += GLOW_STRENGTH * GLOW_COLOR * (GLOW_MAX_DST - dst_rmg)/ ((dst_acc + 1.) / 10.) * ref * ref; // glow
  }
  if (into)
    return color * 0.25;
  return color;
}

void main()
{
  vec2 frag_coord = (vertex_position.xy / 2 + 0.5) * screen_resolution;
//   vec4 tex_color = texture2D(texture, vec2(frag_coord.xy / screen_resolution.xy));
// 
//   vec2 n_coord = gl_FragCoord.xy / (screen_resolution.xy * vec2(0.5, 0.5)) + 0.9;
//   vec2 r_coord = n_coord * screen_resolution.xy;
//   float stime = abs(cos(global_time / 5 + r_coord.x + r_coord.y) * 5 + sin(global_time / 5) * 15) + 20;
//   color_1 = (vec4(0.5 * abs(cos(stime * n_coord.y * n_coord.x)) * 0.25 * abs(sin(stime / n_coord.x* n_coord.y * 10)), 0., 0., 0.));
//   color_1 = color_1.xxxw * 8. - vec4((tex_color.rgb * tex_color.a).rgb, 1.);
//   color_1 = color_1.xxxw * 4. * vec4((tex_color.rgb * tex_color.a).rgb, 1.);

  vec2 uv = (vertex_position.xy); // some good trick for a fs quad :)
  vec3 dir = normalize(vec3(uv * 1.0, 1.0));

  mat3 rotmat = rotationMatrixY(mod(global_time * ROTATION_SPEED, 2 * 3.1415927));

  vec3 pos = vec3(0., 5., -1. * CAM_DISTANCE + sin(global_time) * CAM_DISTANCE_MVT);
  color_1 = vec4(raymarch(pos * rotmat, dir * rotmat), 1.0);

}


