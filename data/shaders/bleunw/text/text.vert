
#version 330 core

#line 4

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// inputs --> to be forwarded
layout(location = 1) in vec2 i_lower_pos;
layout(location = 2) in vec2 i_upper_pos;
layout(location = 3) in vec2 i_dt;
layout(location = 4) in vec2 i_left_top;
// layout(location = 5) in float i_x_inc;

uniform mat4 vp_matrix;
uniform mat4 object_matrix;

// x and y displacement
out mat4 vpo;
out vec3 v_font_scale;

out vec2 lower_pos;
out vec2 upper_pos;
out vec2 dt;
out vec2 left_top;
out float x_inc;

out gl_PerVertex
{
  vec4 gl_Position;
};

void KLMB_MAIN_FUNCTION(void)
{
  // matrix computation
  vpo = (vp_matrix * object_matrix);

  // extract the scale componant from the object matrix
  // (the matrix should be a 'normal' matrix ;) )
  v_font_scale = vec3(length(object_matrix[0]), length(object_matrix[1]), length(object_matrix[2]));

  // output
  gl_Position = vec4(0, 0, 0, 1.);

  // forwarding
  lower_pos = i_lower_pos;
  upper_pos = i_upper_pos;
  dt = i_dt;
  left_top = i_left_top;
//   x_inc = i_x_inc;
}

