

// test fragment shader
// to see if yaggler works ;)

// Specify which version of GLSL we are using.

#version 130

uniform float global_time;
uniform vec2 screen_resolution;
uniform sampler2D texture_sampler;
uniform sampler2D fg_texture_sampler;

out vec4 output_color;
in vec4  vertex_position;

// defines
#ifndef RM_STEP
#define RM_STEP         55.
#endif

#define RM_INC          0.00435

// consts
const float init_time = 5.;
const vec3 origin = vec3(0.,1.11,0.);

// code (minefield)
float length8(in vec3 v)
{
  return pow(pow(v.x, 8.0) + pow(v.y, 8.0) + pow(v.z, 8.0), 1. / 8.);
}
float length8(in vec2 v)
{
  return pow(pow(v.x, 8.0) + pow(v.y, 8.0), 1. / 8.);
}

float d(vec3 p, float r)
{
  vec3 c = vec3(30. /** (cos(p.z / 50.) + 1.0)*/, 30. /** (cos(p.x / 60. + sin(global_time)) + 1.0)*/, 30.);
 p = p + vec3(-20. * cos(global_time / 5.), -3.0 + 20. * sin(global_time / 5.) , -6. + global_time * 13.);
  p = p + vec3(-5, -6, -6. + global_time * 3.);
  p =  mod(p, c) - 0.5 * c;

  float d1 = length(max(abs(p /*+ vec3(0, -0.7, 0.5)*/) - vec3(r, r, r), 0.00));
  vec3 q = abs(p);
  float d2 = max(q.z-r*1.1,max(q.x+q.y*0.57735,q.y*1.1547)-r*1.1);

  float d3 = length(p ) - r;

//    return d2;
   return max(d1, -d2)/*+ 0.01*/;
  return mix(d2, d1, (sin(global_time / 8.)+1.) / 2.);
}

float map(vec3 p)
{
  return d(p, 2.);
}

vec3 normal(in vec3 pos)
{
  vec2 eps = vec2(0.001, 0.0);
  vec3 nor = vec3(
      map(pos + eps.xyy) - map(pos - eps.xyy),
      map(pos + eps.yxy) - map(pos - eps.yxy),
      map(pos + eps.yyx) - map(pos - eps.yyx));
  return normalize(nor);
}

vec3 raymarch(in vec3 from, in vec3 dir, in vec3 ocolor, const float max_it)
{
  vec3 color = vec3(0, 0, 0);

  float dstacc = 0.;
  float rst;
  float td = 0.;
  float c_fact = 1.;
  for (float it = 0.; it < max_it && c_fact > 0.009; ++it)
  {
    rst = map(from + dir * dstacc);
    dstacc += (rst);

//     dir = refract(dir, vec3(0, 0, -1), 1.00101);

    if (rst < 0.001 && td != 1.)
    {
//       color += vec3(0.19, 0.9, 0.001 + 1./length(from)/250. ) * (c_fact) * (1./length(from) / 50.);
      color += vec3(abs(vec3(150., 125. + 25.*cos(global_time), 125. + 25.*sin(global_time)) - from.xyz) / 569.) * (c_fact);
      c_fact *= 0.22;
      td = 1.;
      return color;
//        color = (normal(from + dir * dstacc) + vec3(1., 1., 1.)) / 2.;break;
break;
//       vec3 tdr = reflect(dir, normal(from + dir * dstacc));
       vec3 tdr = refract(dir, normal(from + dir * dstacc), 1.000);
//       dstacc += 0.01;
      from = from + dir * dstacc;
      dir = tdr;
//       break;
//        color += vec3(0.00050, 0.00001, 0.00001) * 1.*(0.01-rst);
      //color *= 1.035;
      //dstacc += (0.5) / 1.10;
    }
    else
    {
      if (rst < 0.001 && td == 1.)
      {
//              color += vec3((vec3(150, 125 + 25.*cos(global_time), 125 + 25.*sin(global_time)) - from.xyz) / 569.) * (c_fact);

//       color += vec3(1., 0., 0.);
//         dstacc += -2.*rst;
//       dstacc += RM_INC;
//         color += vec3(0.01, 0.0001, 0.001) * c_fact;
      }
      else
      {
        color += (1.-td) * (0.0009 + ((1.0 - (clamp(rst + 1.0, 1.0, 2.0) - 1.))) / (100. + 70. * cos(global_time))) * c_fact; // depth + glow
        td = 0.;
      }
    }
  }
  return color + ocolor;
}

vec4 color_init(in vec2 uv)
{
  float t = clamp(global_time, 0., init_time) / init_time;
  float invt = 1.0 - t;

//   float ct = 

  //return vec4(invt*uv.xy + vec2(t, t), invt + t, 1. - clamp(t - .9, 0., 0.1) * 10.);
  return vec4(0., 0., 0., 1. - t);
}

vec2 ripple(in vec2 uv)
{
   uv.x = uv.x+sin(uv.x)/5.;
   uv.y = uv.y+sin(uv.y)/5.;
   return uv;
}


void main()
{
  vec2 frag_coord = (vertex_position.xy / 2 + 0.5) * screen_resolution;
  vec4 tcolor = texture2D(texture_sampler, vec2(frag_coord.xy * 5 / screen_resolution.xy));
  vec4 fgcolor = texture2D(fg_texture_sampler, vec2(((frag_coord.xy / (screen_resolution / 250)) + vec2(sin(global_time / 1.5) * 250 + 250, 0) + vec2(0, cos(global_time) * 250 + 250)) / screen_resolution.xy));

  tcolor.rgb *= tcolor.a; // blend alpha
  fgcolor.rgb *= fgcolor.a; // blend alpha
  fgcolor.a = 1;

  vec2 uv = frag_coord.xy / screen_resolution.xy * 2.0 - 1.0;

  uv.y *= screen_resolution.y / screen_resolution.x;
  vec3 from = origin;

  //vec3 color = raymarch(from, dir);
  uv = ripple(uv);
  vec3 dir = normalize(vec3(uv * 0.8 , 1.0));
  vec4 color = color_init(uv * global_time);

  color = color * color.w + vec4(raymarch(from, dir, tcolor.rgb, RM_STEP), 1.0) * (1. - color.w);

  // set the color.
  output_color =  (fgcolor) / vec4(color.xyz, 1.0) ;
//   output_color = normalize(output_color) * 2;
}
