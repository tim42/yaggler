
#version 330 core

#line 4

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT



in vec3 in_position;

out vec3 orig_vertex_position;
out vec3 vertex_position;

uniform mat4 vp_matrix;
uniform mat4 object_matrix;

out gl_PerVertex
{
    vec4 gl_Position;
};

void KLMB_MAIN_FUNCTION(void)
{
    gl_Position = (vp_matrix * object_matrix) * vec4(in_position, 1.);
    orig_vertex_position = in_position;
    tvertex_position = gl_Position.xyz / gl_Position.w;
}

