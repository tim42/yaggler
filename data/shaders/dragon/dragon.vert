
#version 330 core


in vec3 in_position;
uniform mat4 vp_matrix;
uniform mat4 object_matrix;

out vec4 vertex_position;

void main(void)
{
    gl_Position = (vp_matrix * object_matrix) * vec4(in_position, 1.);
    vertex_position = vec4(in_position, 1.0);
}

