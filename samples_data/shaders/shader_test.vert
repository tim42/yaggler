
#version 130

in vec3 in_position;

out vec4 vertex_position;

void main(void)
{
    gl_Position = vec4(in_position.x , in_position.y, in_position.z + in_position.y, 1.0);
    vertex_position = gl_Position;
}

