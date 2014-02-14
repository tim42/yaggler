
#version 330


uniform float global_time;

in vec3 in_position;
uniform mat4 vp_matrix;
uniform mat4 object_matrix;

out vec4 vertex_position;

void main(void)
{
//     gl_Position = vec4(in_position.x , in_position.y, in_position.z + in_position.y* abs(cos(global_time*0.9)), 1.0);
//     gl_Position = (vp_matrix * object_matrix) * vec4(in_position, 1.);
    gl_Position = (vp_matrix * object_matrix) * vec4(in_position, 1.);
    vertex_position = vec4(in_position, 1.0);//;(gl_Position - 0.250) / 2.0;
}
