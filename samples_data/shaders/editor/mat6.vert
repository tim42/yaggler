
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

out vec3 orig_vertex_position;
out vec4 vertex_position;

uniform float global_time;

out gl_PerVertex
{
    vec4 gl_Position;
};

void KLMB_MAIN_FUNCTION(void)
{
    vec3 in_tmp_position = in_position;
    in_tmp_position.x *= ((cos((sin(global_time * 2) / 5. + in_position.y) * 20.)) + 2.) / 2.0;
//     in_tmp_position.x += ((sin((sin(global_time) / 5. * in_position.y + in_position.y) * 40.)) + 2.) / 20.0;
    in_tmp_position.z *= ((cos((sin(global_time) / 5. + in_position.y) * 20.)) + 2.) / 2.0;
    gl_Position = (vp_matrix * object_matrix) * vec4(in_tmp_position, 1.);
    vertex_position = ((object_matrix) * vec4(in_tmp_position, 1.));
    orig_vertex_position = (in_tmp_position);
}

