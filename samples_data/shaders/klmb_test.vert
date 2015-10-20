
#version 330 core

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

in vec3 in_position;

out vec4 vertex_position;

void KLMB_MAIN_FUNCTION(void)
{
    gl_Position = vec4(in_position.x , in_position.y, in_position.z + in_position.y, 1.0);
    vertex_position = gl_Position;
}
