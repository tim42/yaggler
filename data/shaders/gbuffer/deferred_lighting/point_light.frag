
#version 330 core

#line 4

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            2

// MATHS
#define PI 3.14159265358979323846264338

uniform vec2 screen_resolution;
uniform float selection_factor;

uniform bool show_light_geom;
uniform vec3 light_pos;
uniform float light_radius;
uniform vec3 cam_pos;

const vec3 light_falloff = vec3(0.0000, 0.01, 2.);
uniform vec3 light_color = vec3(1., 1., 1.);

uniform sampler2D scene_color;          // color:  RGB   sel:   A
uniform sampler2D lit_scene_color;      // color:  RGB   sel:   A
uniform sampler2D geometry;             // normal: RGB   depth: A

uniform mat4 cam_view;
uniform mat4 vp_matrix;
uniform mat4 object_matrix;

in vec4 gbuffer_position;
in vec4 gbuffer_vertex_position;
in vec3 gbuffer_normal;
in vec3 tri_distance;


KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0); // color:  RGB   sel:   A
KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_1); // color:  RGB   sel:   A

float amplify(float d, float scale, float offset)
{
  d = scale * d + offset;
  d = clamp(d, 0., 1.);
  d = 1. - exp2(-2. * d * d);
  return d;
}

void KLMB_MAIN_FUNCTION()
{
  // get screen uv
  vec2 inv_buffer_size = 1. / screen_resolution;
  vec2 tuv = gl_FragCoord.xy * inv_buffer_size;

  // get scene color and geometry
  vec4 scene = texture(scene_color, tuv);
  vec4 lit_scene = texture(lit_scene_color, tuv);
  vec4 geom = texture(geometry, tuv);

  // dir fix
  vec2 _uv = 2. * (gl_FragCoord.xy) * inv_buffer_size - 1.;
  _uv.x *= screen_resolution.x * inv_buffer_size.y;
  float _w = tan((PI / 4.) / 2. );
  vec3 _rdir = (vec3(_uv, 0.50 / _w));
  float _k = 0.5825 + ( _w / (_rdir.z) * length(_rdir));

  // compute the direction and worl position
  vec3 gb = gbuffer_vertex_position.xyz / gbuffer_vertex_position.w;
  vec3 dir = normalize((gb - cam_pos.xyz));

  vec3 pos = cam_pos.xyz + (dir) * geom.w * (_k);

  vec3 obj_to_light = light_pos.xyz - pos.xyz;
  float dst = length(obj_to_light);

  // light geometry
  float d1 = min(min(tri_distance.x, tri_distance.y), tri_distance.z);
  d1 = clamp(amplify(d1, 8., 0.0), 0., 100.) * 1.3;

  if (dst >= light_radius && (!show_light_geom || (d1 > 0.20 || (geom.w > 0 && geom.w < gbuffer_position.z))))
    discard;

  // compute the lit scene
  dst = dst / light_radius;
  vec3 total_light_contrib = max(0.0, dot(obj_to_light, -geom.xyz)) * light_color.rgb;
  total_light_contrib /= dot(light_falloff.xyz, vec3(1.0, dst, dst * dst));
  scene.rgb *= total_light_contrib * selection_factor * .35 * (1.-dst*dst);


  // cut holes, render geometry
  if (show_light_geom && d1 < 0.20 && (geom.w == 0. || geom.w > gbuffer_position.z))
  {
    geom = vec4(gbuffer_normal, gbuffer_position.z);
    if (d1 < 0.10)
      scene = vec4(light_color, selection_factor);
    else
      scene = vec4(1. - light_color, selection_factor);
  }

  // write them back
  KLMB_SHARED_NAME(color_0).rgb = lit_scene.rgb + scene.rgb;
  KLMB_SHARED_NAME(color_0).a = max(lit_scene.a, scene.a);
  KLMB_SHARED_NAME(color_1) = geom;
}
