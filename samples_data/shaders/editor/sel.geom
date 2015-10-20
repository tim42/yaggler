
#version 330 core

#line 4

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT


layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// inputs
in gl_PerVertex
{
    vec4 gl_Position;
} gl_in[];

in vec4 orig_vertex_position[];

// outputs
out gl_PerVertex
{
    vec4 gl_Position;
};

out vec3 tri_distance;

// uniforms
uniform float global_time;

// external functions (this is a gbuffer geom shader)
void gbuffer_set_position(vec4 _position);
vec3 gbuffer_compute_normal(vec4 v1, vec4 v2, vec4 v3);

// main
void KLMB_MAIN_FUNCTION()
{
  vec3 normal = gbuffer_compute_normal(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);

  vec4 n = vec4(0, 0, 0, 0);

  gl_Position = gl_in[0].gl_Position + n;
  gl_Position.w = gl_in[0].gl_Position.w + orig_vertex_position[0].w;
  gbuffer_set_position(gl_Position);
  tri_distance = vec3(1, 0, 0);
  EmitVertex();

  gl_Position = gl_in[1].gl_Position + n;
  gl_Position.w = gl_in[1].gl_Position.w + orig_vertex_position[1].w;
  gbuffer_set_position(gl_Position);
  tri_distance = vec3(0, 1, 0);
  EmitVertex();

  gl_Position = gl_in[2].gl_Position + n;
  gl_Position.w = gl_in[2].gl_Position.w + orig_vertex_position[2].w;
  gbuffer_set_position(gl_Position);
  tri_distance = vec3(0, 0, 1);
  EmitVertex();


  EndPrimitive();
}
