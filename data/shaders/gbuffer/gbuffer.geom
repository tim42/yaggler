
#version 330 core

#line 4

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// there's only this shader for the geom stage
#if KLMB_PROG_NUMBER == 1

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// inputs
in gl_PerVertex
{
  vec4 gl_Position;
} gl_in[];


// outputs
out gl_PerVertex
{
  vec4 gl_Position;
};

out vec3 tri_distance;
in vec4 v_position[];
in vec4 t_position[];

#endif


// to the frag stage
out vec3 gbuffer_normal;
out vec3 gbuffer_oriented_normal;
out vec4 gbuffer_position;
out vec4 gbuffer_vertex_position;
out vec4 gbuffer_vertex_orig_position;

uniform mat4 view_matrix;
uniform mat4 object_matrix;

// NOTE: those two functions must be called by any other geom shader (if any).
void gbuffer_set_position(vec4 _position);
vec3 gbuffer_compute_normal(vec4 v1, vec4 v2, vec4 v3);

void gbuffer_set_position(vec4 _position)
{
  gbuffer_position = _position;
}

vec3 gbuffer_compute_normal(vec4 v1, vec4 v2, vec4 v3)
{
  gbuffer_normal = normalize(cross((v3 - v1).xyz, (v2 - v1).xyz));

  // nice effects:
//     gbuffer_oriented_normal = normalize(vec4(gbuffer_normal, 0.) * inverse(view_matrix)* inverse(object_matrix) ).xyz;
  return gbuffer_normal;
}

vec3 gbuffer_compute_oriented_normal(vec4 v1, vec4 v2, vec4 v3)
{
  vec3 tnormal = normalize(cross((v3 - v1).xyz, (v2 - v1).xyz));
  gbuffer_oriented_normal = normalize(object_matrix * vec4(tnormal, 0.)).xyz;
  return gbuffer_oriented_normal;
}

void KLMB_MAIN_FUNCTION()
{
#if KLMB_PROG_NUMBER == 1

  gbuffer_compute_oriented_normal(t_position[0], t_position[1], t_position[2]);
  vec3 normal = gbuffer_compute_normal(v_position[0], v_position[1], v_position[2]);
//   vec3 normal = gbuffer_compute_normal(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
  gbuffer_vertex_position = v_position[0];
  gbuffer_vertex_orig_position = t_position[0];
  tri_distance = vec3(1, 0, 0);
  gbuffer_set_position(gl_in[0].gl_Position);
  gl_Position = gl_in[0].gl_Position;
  EmitVertex();


//   gbuffer_compute_normal(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
  gbuffer_vertex_position = v_position[1];
  gbuffer_vertex_orig_position = t_position[1];
  gbuffer_normal = normal;
  tri_distance = vec3(0, 1, 0);
  gbuffer_set_position(gl_in[1].gl_Position);
  gl_Position = gl_in[1].gl_Position;
  EmitVertex();


//   gbuffer_compute_normal(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
  gbuffer_vertex_position = v_position[2];
  gbuffer_vertex_orig_position = t_position[2];
  gbuffer_normal = normal;
  tri_distance = vec3(0, 0, 1);
  gbuffer_set_position(gl_in[2].gl_Position);
  gl_Position = gl_in[2].gl_Position;
  EmitVertex();

  EndPrimitive();
#endif
}

