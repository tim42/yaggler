
#version 330 core


in vec3 in_position;
uniform mat4 vp_matrix;
uniform mat4 object_matrix;

// out vec4 vertex_position;

uniform float global_time;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main(void)
{
    gl_Position = (vp_matrix * object_matrix) * vec4(in_position, 1.);
//     gl_Position.x +=  cos(global_time * 5 + (50 * in_position.y)) * length(in_position) * 5;
//     gl_Position.y +=  sin(global_time * 5 + (50 * in_position.x)) * length(in_position) * 5;
//     vertex_position = vec4(in_position, 1.0);
}

