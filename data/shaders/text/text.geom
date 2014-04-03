
#version 330 core

#line 4

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// in/out layout
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

// inputs
in gl_PerVertex
{
  vec4 gl_Position;
} gl_in[];

in mat4 vpo[];

in vec2 lower_pos[];
in vec2 upper_pos[];
in vec2 left_top[];
in vec2 dt[];
in vec3 v_font_scale[];

// outputs
out gl_PerVertex
{
  vec4 gl_Position;
};

out vec3 font_scale;
out vec2 uv;

// uniforms
layout(std140) uniform displacement_block
{
  vec4 xy_pos[1];
};

// main
void KLMB_MAIN_FUNCTION()
{
  // get the x displacement
  int _dec = (gl_PrimitiveIDIn % 2) * 2;
  float xpos = xy_pos[gl_PrimitiveIDIn >> 1][_dec + 0];
  float ypos = xy_pos[gl_PrimitiveIDIn >> 1][_dec + 1];

  float x_dec = left_top[0].x - dt[0].x + xpos;
  float y_dec = left_top[0].y + dt[0].y - ypos;

  // output vertices (a quad)
  uv = vec2(upper_pos[0].x, lower_pos[0].y);
  font_scale = v_font_scale[0];
  gl_Position = vpo[0] * vec4(1.0 + x_dec, 1.0 + y_dec, 0, 1.);
  EmitVertex();

  uv = vec2(upper_pos[0].x, upper_pos[0].y);
  font_scale = v_font_scale[0];
  gl_Position = vpo[0] * vec4(1.0 + x_dec, 0.0 + y_dec, 0, 1.);
  EmitVertex();

  uv = vec2(lower_pos[0].x, lower_pos[0].y);
  font_scale = v_font_scale[0];
  gl_Position = vpo[0] * vec4(0.0 + x_dec, 1.0 + y_dec, 0, 1.);
  EmitVertex();

  uv = vec2(lower_pos[0].x, upper_pos[0].y);
  font_scale = v_font_scale[0];
  gl_Position = vpo[0] * vec4(0.0 + x_dec, 0.0 + y_dec, 0, 1.);
  EmitVertex();

  EndPrimitive();
}
