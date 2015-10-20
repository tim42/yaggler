
// test fragment shader
// to see if YäGLer works ;)

#version 130

uniform float global_time;
uniform vec2 screen_resolution;
uniform sampler2D texture_sampler;

out vec4 output_color;
in vec4  vertex_position;

void main()
{
  vec2 frag_coord = (vertex_position.xy / 2. + 0.5) * screen_resolution;

  vec2 tsize = textureSize(texture_sampler, 0);
  vec2 slice_size = tsize / 3.;

  vec2 linear_coords = (frag_coord.xy - slice_size) / (screen_resolution.xy - 2. * slice_size) * (1./3.) + (1./3.);
  vec2 begin_const_coords = vec2(frag_coord.xy / tsize.xy);
  vec2 end_const_coords = vec2((tsize - (screen_resolution.xy - frag_coord.xy)) / tsize.xy);

  vec2 coords = linear_coords;

  if (frag_coord.x < slice_size.x)
    coords.x = begin_const_coords.x;
  else if (frag_coord.x > screen_resolution.x - slice_size.x)
    coords.x = end_const_coords.x;

  if (frag_coord.y < slice_size.y)
    coords.y = begin_const_coords.y;
  else if (frag_coord.y > screen_resolution.y - slice_size.y)
    coords.y = end_const_coords.y;

  vec4 tcolor = texture2D(texture_sampler, coords, 0);

  // compute alpha
  tcolor.rgb *= tcolor.a;

  // blend alpha with bg color (0.65)
  output_color = tcolor + (1. - tcolor.a) * 0.65;
}
