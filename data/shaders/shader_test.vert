
#version 130

in vec3 in_position;

out vec4 vertex_position;

uniform float global_time;

void main(void)
{
//    gl_Position = ftransform();
//       gl_Position = vec4(in_Position.x * abs(sin(iGlobalTime+1.)), in_Position.y* abs(cos(iGlobalTime*0.9)), in_Position.z + in_Position.y* abs(cos(iGlobalTime*0.9)), 1.0);
    gl_Position = vec4(in_position.x , in_position.y, in_position.z + in_position.y* abs(cos(global_time*0.9)), 1.0);
    vertex_position = gl_Position;
}

