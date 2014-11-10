

// test klmb fragment shader
// to see if yaggler works ;)

// Specify which version of GLSL we are using.

#version 440 core

#line 10

// #ifndef USING_KLMB_FRAMEWORK
// # error This shader must be used with the K:LMB framework
// #endif

uniform float global_time;
uniform vec2 buffer_size;

// the 3D light hit point map
layout(rgba32f, binding = 0) restrict coherent uniform image3D light_space_map;

in vec4 orig_vertex_position;

#ifdef USING_KLMB_FRAMEWORK

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0);


// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

// could be dangerous (x computed multiple times). SO PAY ATTENTION !!!
#define DANGEROUS_POW2(x)         ((x) * (x))

struct ray
{
  vec3 p;
  vec3 d;
};

struct material
{
  vec3 color;
  float trsp; // "transparancy" factor.
  float diff; // diffraction indice.
  float emit; // [0-1] emmissive. (kill rate after hit)
  float kemit; // [0-1] emmissive. (kill rate after hit)
};

struct hit_point
{
  float d;
  vec3 n;
  vec3 p;
  material m;
};

// NOTE: r.d must be normalized.
// could be fast.
// inspired from: https://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter1.htm
hit_point sphere_int(ray r, vec3 pos, float radius)
{
  vec3 rpos = r.p - pos;
  r.d = normalize(r.d);

  float b = 2. * (r.d.x * rpos.x + r.d.y * rpos.y + r.d.z * rpos.z);
  float c = dot(rpos, rpos) - DANGEROUS_POW2(radius);
  float dt = DANGEROUS_POW2(b) - 4. * c ;

  hit_point hp;
  hp.d = 100000000.;

  if (dt > 0.) // screw equality
  {
    float i = 1;
    dt = sqrt(dt);
    hp.d = (-b - dt);
    if (hp.d <= 0.)
    {
      hp.d = (-b + dt);
      i = -1;
    }
    hp.d = hp.d * 0.5;
    hp.p = hp.d * r.d + r.p;
    hp.n = i * (hp.p - pos) / radius; // normal is normalized.
  }
  return hp;
}

// NOTE: r.d must be normalized
// NOTE: n must be normalized in order to have an output n normalized.
hit_point plane_int(ray r, vec3 pos, vec3 n)
{
  vec3 rpos = r.p - pos;
  float d1 = dot(n, r.d);
  float d2 = -(dot(n, rpos));

  hit_point hp;
  hp.d = 100000000.;
  if (d1 != 0.)
    hp.d = d2 / d1;
  hp.n = n * -(sign(hp.d));
  hp.p = r.d * hp.d + r.p;

  return hp;
}

// the scene (a cornell_box, a bit modified)
hit_point cornell_box(ray r)
{
  // spheres
  hit_point ret  = sphere_int(r, vec3(-4., -5.9, -13.), 3.);
  ret.m.color = vec3(0.85, 0.3, 0.0);
  ret.m.color = vec3(0.50, 0.50, 0.99);
  ret.m.trsp = 0.000;
  ret.m.diff = 0.7;
  ret.m.emit = 0.0;
  ret.m.kemit = 0.9;
  hit_point sph2 = sphere_int(r, vec3(4., -6.4/*+ (sin(global_time / 5.) + 1) * 2*/, -10./* + (sin(global_time / 5.) + 1) * 2*/), 2.5);
  hit_point sph3 = sphere_int(r, vec3(-4.+ (sin(global_time / 5.) + 1) * 4, -8.0, -5./* + (sin(global_time / 5.) + 1) * 2*/), 1.0);

  // planes
  hit_point back =   plane_int(r, vec3(0, 0, -19), vec3(0, 0, 1));
  hit_point left =   plane_int(r, vec3(-9, 0, 0),  vec3(1, 0, 0));
  hit_point top =    plane_int(r, vec3(0, +9, 0),  vec3(0, -1, 0));
  hit_point right =  plane_int(r, vec3(+9, 0, 0),  vec3(-1, 0, 0));
  hit_point ground = plane_int(r, vec3(0, -9, 0),  vec3(0, 1, 0));

  // real hit_point:
  if ((sph2.d < ret.d && sph2.d > 0.01) || ret.d < 0.01)
  {
    ret = sph2;
    ret.m.color = vec3(0.5, 0.5, 1.0);
      ret.m.color = vec3(1.1, 0.8, 0.5);
    ret.m.trsp = 0.0;
    ret.m.diff = 0.83;
    ret.m.emit = 0.8;
    ret.m.kemit = 0.9;
  }
  if ((sph3.d < ret.d && sph3.d > 0.01) || ret.d < 0.01)
  {
    ret = sph3;
    ret.m.color = vec3(1.1, 0.8, 0.5);
    ret.m.color = vec3(1.0, 0.8, 0.5);
    ret.m.trsp = 0.9;
    ret.m.diff = 0.83;
    ret.m.emit = 1.0;
    ret.m.kemit = 0.9;
  }
  if ((back.d < ret.d && back.d > 0.01) || ret.d < 0.01)
  {
    ret = back;
    ret.m.color = vec3(0.9, 0.9, 0.9);
    ret.m.trsp = 0.0;
    ret.m.diff = 0;
    ret.m.emit = 1.0;
    ret.m.kemit = 0.90;
  }
  if ((left.d < ret.d && left.d > 0.01) || ret.d < 0.01)
  {
    ret = left;
    ret.m.color = vec3(1.2, 0.3, 0.3);
//     ret.m.color = vec3(0.9, 0.9, 0.9);
    ret.m.trsp = 0;
    ret.m.diff = 0;
//     ret.m.emit = 0.0;//95;
    ret.m.emit = 0.99;//95;
    ret.m.kemit = 1.00;
  }
  if ((top.d < ret.d && top.d > 0.01) || ret.d < 0.01)
  {
    ret = top;
    ret.m.color = vec3(0.9, 0.9, 0.9);
    ret.m.trsp = 0;
    ret.m.diff = 0;
    ret.m.emit = 0.;
    ret.m.kemit = 0.90;
  }
  if ((right.d < ret.d && right.d > 0.01) || ret.d < 0.01)
  {
    ret = right;
    ret.m.color = vec3(0.3, 1.2, 0.3);
    ret.m.trsp = 0;
    ret.m.diff = 0;
    ret.m.emit = 0.990;
    ret.m.kemit = 1.00;
  }
  if ((ground.d < ret.d && ground.d > 0.01) || ret.d < 0.01)
  {
    ret = ground;
    ret.m.color = vec3(1.0, 1.0, 1.0);
    ret.m.trsp = 0;
    ret.m.diff = 0;
    ret.m.emit = 0.0;
    ret.m.kemit = 0.90;
  }
  if (ret.d < 0.01)
  {
    ret.d = 10000.;
    ret.m.emit = 0;
    ret.m.trsp = 0;
    ret.m.color = vec3(0, 0, 0);
  }
  return ret;
}

// from IQ https://www.shadertoy.com/view/MdSGDm
vec4 hash4( float n ) { return fract(sin(vec4(n,n+2.0,n+3.0,n+5.0))*vec4(43758.5453123,22578.1459123,19642.3490423,85124.04831)) * 2. - 1.; }



ray point_light(vec3 pos, vec2 uv)
{
  ray r;
  r.p = pos;
  r.d = (hash4(abs(fract(global_time) + uv.x - uv.y + gl_FragCoord.x + gl_FragCoord.y + gl_FragCoord.y * uv.x)).xyz) + fract(global_time + uv.x + uv.y) - 0.5;
  return r;
}
ray spot_light(vec3 pos, vec3 d, float angle, vec2 uv)
{
  ray r;
  r.p = pos;
  r.d = reflect(d, d + hash4(abs(cos(global_time) + uv.x - uv.y + gl_FragCoord.x + gl_FragCoord.y + gl_FragCoord.y * uv.x)).xyz / angle);
  return r;
}

#define PATH_TRACER_TSZ         202
#define PATH_TRACER_LOOPS       2
#define PATH_TRACER_MAX_STEPS   30
#define _PATH_TRACER_PRECISION  .003

#define _NP_PATH_TRACER_FADER(x, y)      x##y
#define _NE_PATH_TRACER_FADER(x, y)      _NP_PATH_TRACER_FADER(x, y)
#define _PATH_TRACER_FADER_               _NE_PATH_TRACER_FADER(1, _PATH_TRACER_PRECISION)

float PATH_TRACER_PRECISION = 1. / (2. + global_time*global_time) + _PATH_TRACER_PRECISION;
float _PATH_TRACER_FADER = 1. + PATH_TRACER_PRECISION;

void accum_light(vec3 pos, vec3 light_color, float accum)
{
  vec3 space_uv_conv = clamp(vec3((pos.x + 10) / 20., (pos.y + 10) / 20., (-pos.z) / 20.) * PATH_TRACER_TSZ, -2, PATH_TRACER_TSZ + 2);
  vec4 o_light_color= imageLoad(light_space_map, ivec3(space_uv_conv));
  vec4 res = vec4((o_light_color.xyz / _PATH_TRACER_FADER + light_color * accum), o_light_color.w + 1);
  imageStore(light_space_map, ivec3(space_uv_conv), res);

//   res *= 0.5;
//   imageStore(light_space_map, ivec3(space_uv_conv) + ivec3( 1, 0, 0), res + 0.5 * imageLoad(light_space_map, ivec3(space_uv_conv) + ivec3( 1, 0, 0)));
//   imageStore(light_space_map, ivec3(space_uv_conv) + ivec3(-1, 0, 0), res + 0.5 * imageLoad(light_space_map, ivec3(space_uv_conv) + ivec3(-1, 0, 0)));
// 
//   imageStore(light_space_map, ivec3(space_uv_conv) + ivec3(0,  1, 0), res + 0.5 * imageLoad(light_space_map, ivec3(space_uv_conv) + ivec3(0,  1, 0)));
//   imageStore(light_space_map, ivec3(space_uv_conv) + ivec3(0, -1, 0), res + 0.5 * imageLoad(light_space_map, ivec3(space_uv_conv) + ivec3(0, -1, 0)));
// 
//   imageStore(light_space_map, ivec3(space_uv_conv) + ivec3(0, 0,  1), res + 0.5 * imageLoad(light_space_map, ivec3(space_uv_conv) + ivec3(0, 0,  1)));
//   imageStore(light_space_map, ivec3(space_uv_conv) + ivec3(0, 0, -1), res + 0.5 * imageLoad(light_space_map, ivec3(space_uv_conv) + ivec3(0, 0, -1)));
}
// run the lighter
void run_lighter(vec2 uv)
{
  uv = (uv) * 2.;
  for (int i = 0; i < PATH_TRACER_LOOPS; ++i)
  {
    ray r;
//       r = point_light(vec3(0, -5., -5.0 /*+ cos(global_time) * 5.*/), uv);
    float spt = abs(hash4(sin(global_time * i) + gl_FragCoord.x + gl_FragCoord.y).x);
    if (spt > 0.6)
      r = spot_light(vec3(0, -8., -0.8 /*+ cos(global_time) * 5.*/), (vec3(/*cos(global_time/5.)*/0, 0, /*(sin(global_time/5.))*/1)), 2.9, uv);
    else if (spt > 0.3)
//       r = spot_light(vec3(0, 4., -18. /*+ cos(global_time) * 5.*/), (vec3(-0.5, 0.25, 0)), 20., uv);
      r = spot_light(vec3(0, 4., -18. /*+ cos(global_time) * 5.*/), (vec3(cos(global_time/2.), (sin(global_time/2.)), 0)), 7.9, uv);
    else if (spt > 0.0)
      r = spot_light(vec3(0, 5., -18.8 /*+ cos(global_time) * 5.*/), vec3(0, -1, 0), 2.9, uv);
// //     else
// //       r = point_light(vec3(0, 6., -5.0 /*+ cos(global_time) * 5.*/), uv);
//     r = spot_light(vec3(0, 8., -5.8 /*+ cos(global_time) * 5.*/), (vec3(/*cos(global_time/5.)*/0, 1, /*(sin(global_time/5.))*/0)), 1, uv);

    vec3 color = vec3(1, 1, 1);

    float accum = 1.; // (accum == 0) <==> killed

    for (int j = 0; j < PATH_TRACER_MAX_STEPS && accum > 0.01; ++j)
    {
      hit_point hp = cornell_box(r);

      vec4 rnd = hash4((accum + sin(hp.p.x) + sin(gl_FragCoord.x) + cos(i + global_time * j)));
      r.p = hp.p;

      color = color * (hp.m.color * (1. - hp.m.trsp) + color * hp.m.trsp) * accum;
//       if (abs(rnd.x) > 0.5)
//       {
        accum_light(r.p, color * PATH_TRACER_PRECISION, accum);
//         break;
//       }
      if (hp.m.trsp > abs(rnd.x))
      {
        r.p = hp.p + r.d * 0.1;
        r.d = refract(r.d, hp.n, hp.m.diff);
        accum *= hp.m.trsp;
      }
      else
      {
        accum *= hp.m.kemit;
        r.d = reflect(r.d, hp.n + rnd.xyz / 7.);
      }
    }
  }
}

#define PI 3.14159265358979323846264338

void KLMB_MAIN_FUNCTION()
{
  vec2 uv = orig_vertex_position.xy;
  uv.x *= buffer_size.x / buffer_size.y;
  float w = tan((PI / 4.) / 2. );
  vec3 dir = normalize(vec3(uv * 1.0, -1. / w));

  // distance with the plane in z = w
  // (get correct distance)
//   float k = ((-w) / dir.z);
//   dir *= k;

  if (int(gl_FragCoord.x / 128) % 10 == 1)
    run_lighter(uv);
//   memoryBarrierImage();

  ray r;
  r.d = dir;
  r.p = vec3(cos(global_time/5.) * 5., 0, 20 - abs(sin(global_time/5.)) * 5.);
  r.p = vec3(0, 0, 20);
  vec3 color = vec3(0, 0, 0);
  float accum = 1.;
  for (int i = 0; i < 3 && accum > 0.01; ++i)
  {
    hit_point hp = cornell_box(r);

  // image dims: 64 x 64 x 64, space: [-10:10] x [-10:10] x [0:20]
    vec3 space_uv_conv = clamp(vec3((hp.p.x + 10) / 20., (hp.p.y + 10) / 20., (-hp.p.z) / 20.) * PATH_TRACER_TSZ, -2, PATH_TRACER_TSZ + 2);
    vec4 light_color = imageLoad(light_space_map, ivec3(space_uv_conv));
//     imageStore(light_space_map, ivec3(space_uv_conv), light_color / 1.1); // stole photons

    float cnt = 10;
    light_color *= cnt;
    for (int j = 0; j < 0; ++j)
    {
      vec4 hash = hash4(float((j + 1) * gl_FragCoord.x + gl_FragCoord.y * fract(global_time)));
      ivec3 lpos = ivec3(space_uv_conv) + ivec3((hash.xyz) * (2. + 1));
      vec4 tlight_color = imageLoad(light_space_map, lpos);
//       imageStore(light_space_map, lpos, tlight_color / 1.01); // stole photons

      if (length(tlight_color) > 0.01)
      {
        ++cnt;
        light_color += tlight_color;
      }
    }
//     light_color += imageLoad(light_space_map, ivec3(space_uv_conv) + ivec3(1, 0, 0));
//     light_color += imageLoad(light_space_map, ivec3(space_uv_conv) + ivec3(0, 1, 0));
//     light_color += imageLoad(light_space_map, ivec3(space_uv_conv) + ivec3(0, 0, 1));
//     light_color += imageLoad(light_space_map, ivec3(space_uv_conv) - ivec3(1, 0, 0));
//     light_color += imageLoad(light_space_map, ivec3(space_uv_conv) - ivec3(0, 1, 0));
//     light_color += imageLoad(light_space_map, ivec3(space_uv_conv) - ivec3(0, 0, 1));
//     imageStore(light_space_map, ivec3(space_uv_conv) + ivec3(1, 0, 0), light_color / 7.0); // stole photons
//     imageStore(light_space_map, ivec3(space_uv_conv) + ivec3(0, 1, 0), light_color / 7.0); // stole photons
//     imageStore(light_space_map, ivec3(space_uv_conv) + ivec3(0, 0, 1), light_color / 7.0); // stole photons
//     imageStore(light_space_map, ivec3(space_uv_conv) - ivec3(1, 0, 0), light_color / 7.0); // stole photons
//     imageStore(light_space_map, ivec3(space_uv_conv) - ivec3(0, 0, 1), light_color / 7.0); // stole photons
//     imageStore(light_space_map, ivec3(space_uv_conv) - ivec3(0, 1, 0), light_color / 7.0); // stole photons

    color += accum * (light_color.xyz / (cnt-5) /*+ hp.m.color / 7.*/) * (1.-hp.m.trsp);
//     color =  accum * (hp.n / 2. + 0.5) * (1.-hp.m.trsp);
    accum *= hp.m.trsp;
    r.p = hp.p + r.d * 0.1;
    r.d = refract(r.d, hp.n, hp.m.diff);
  }
//   memoryBarrierImage();

  KLMB_SHARED_NAME(color_0).rgb = color;
  KLMB_SHARED_NAME(color_0).a = 1.;
}

#else


// output something... _*red*_ :)
void main()
{
  color_0 = vec4(1., .0, .0, .0);
}

#endif

