
#version 330 core

#line 4

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT


layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in gl_PerVertex
{
    vec4 gl_Position;
} gl_in[];

in vec4 orig_vertex_position[];


out gl_PerVertex
{
    vec4 gl_Position;
};

out vec3 normal;
out vec3 tri_distance;
out vec4 vertex_position;

uniform float global_time;

void KLMB_MAIN_FUNCTION()
{

  normal = normalize(cross((gl_in[2].gl_Position - gl_in[0].gl_Position).xyz, (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz));

  vec4 n = 0.05 / (vec4((normal), orig_vertex_position[0].w)) * abs(1. - cos((global_time/ 2. - length(orig_vertex_position[0]) / 3.5))) * 3.1;
  // n = sign(n) * (sqrt(abs(n) * 0.07));

  gl_Position = gl_in[0].gl_Position /*/ (2.0 + sin(global_time)) */ + n;
  tri_distance = vec3(1, 0, 0);
  vertex_position = gl_Position;
  EmitVertex();

  gl_Position = gl_in[1].gl_Position /*/ (2.0 + cos(2*global_time))*/ + n;
  tri_distance = vec3(0, 1, 0);
  vertex_position = gl_Position;
  EmitVertex();

  gl_Position = gl_in[2].gl_Position /*/ (2.0 + cos(global_time))*/ + n;
  tri_distance = vec3(0, 0, 1);
  vertex_position = gl_Position;
  EmitVertex();


  EndPrimitive();
}
