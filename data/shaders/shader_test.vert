/*----------------- "exampleVertexShader.vert" -----------------*/

// Specify which version of GLSL we are using.
#version 130

// in_Position was bound to attribute index 0("shaderAttribute")
in vec3 in_Position;

out vec4 position;

uniform float iGlobalTime;

void main(void)
{
//    gl_Position = ftransform();
//       gl_Position = vec4(in_Position.x * abs(sin(iGlobalTime+1.)), in_Position.y* abs(cos(iGlobalTime*0.9)), in_Position.z + in_Position.y* abs(cos(iGlobalTime*0.9)), 1.0);
    gl_Position = vec4(in_Position.x , in_Position.y, in_Position.z + in_Position.y* abs(cos(iGlobalTime*0.9)), 1.0);
    position = gl_Position;
}
/*
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main()
{

    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

    // UV of the vertex. No special space for this one.
    UV = vertexUV;
}*/
/*--------------------------------------------------------------*/
