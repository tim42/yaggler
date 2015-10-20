
#version 330 core

#line 4

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT



in vec3 in_position;
uniform mat4 vp_matrix;
uniform mat4 object_matrix;

out vec4 orig_vertex_position;

uniform float global_time;

out gl_PerVertex
{
    vec4 gl_Position;
};

void KLMB_MAIN_FUNCTION(void)
{
    gl_Position = (vp_matrix * object_matrix) * vec4(in_position, 1.);
    orig_vertex_position = vec4(in_position, 1.0);
}

