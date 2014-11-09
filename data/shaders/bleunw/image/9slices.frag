
#version 330 core
#line 3

//
// Scalable 9slices.
// The constant portions of the image keep their proportions but are scaled
// with the image (their scale is equal to min(scale.x, scale.y)).
//



// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT
// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

// output
KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0);

// uniforms
uniform mat4 object_matrix;

uniform sampler2D image;

uniform vec2 start_uv;
uniform vec2 end_uv;

uniform float alpha_factor;


// input [from vertex stage]
in vec3 orig_vertex_position; // [0..1] from model
in vec4 vertex_position;      // in screen pixels

// based on: (S is scale)
// (1 / 3 - (1 / 3 * S - 1 / 3)/2) * 2 + 1 / 3 * S = 1

void KLMB_MAIN_FUNCTION()
{
  vec2 o_uv = vec2(orig_vertex_position.x, 1. - orig_vertex_position.y);
  vec2 img_fact = vec2(length(object_matrix[0]), length(object_matrix[1]));

  vec2 border_sz = min(img_fact.x, img_fact.y) / img_fact / 3.;
  vec2 middle_sz = 1. - border_sz * 2.;

  // linear coefs
  vec2 border_a = (1. / 3.) / border_sz;
  vec2 middle_a = (1. / 3.) / middle_sz;

  // offsets
  const vec2 start_b  = vec2(0, 0);
  const vec2 middle_b = vec2(1. / 3., 1. / 3.);
  const vec2 end_b    = middle_b + vec2(1. / 3., 1. / 3.);

  // uv offsets
  const vec2 uv_start   = vec2(0, 0);
  vec2 uv_middle        = border_sz;
  vec2 uv_end           = uv_middle + middle_sz;

  vec2 coef = border_a;
  vec2 offset = end_b;
  vec2 diff = uv_end;

  // coefs and offset (slice the image)
  if (o_uv.x < uv_middle.x)
  {
    diff.x = uv_start.x;
    offset.x = start_b.x;
  }
  else if (o_uv.x < uv_end.x)
  {
    diff.x = uv_middle.x;
    coef.x = middle_a.x;
    offset.x = middle_b.x;
  }
  if (o_uv.y < uv_middle.y)
  {
    diff.y = uv_start.y;
    offset.y = start_b.y;
  }
  else if (o_uv.y < uv_end.y)
  {
    diff.y = uv_middle.y;
    coef.y = middle_a.y;
    offset.y = middle_b.y;
  }

  // compute the real uv
  vec2 uv = offset + coef * (o_uv - diff);

  // start/end uvs are applied after the slicing.
  vec2 diff_uv = end_uv - start_uv;
  KLMB_SHARED_NAME(color_0) = texture(image, uv * diff_uv + start_uv);
  KLMB_SHARED_NAME(color_0).a *= alpha_factor;
}
