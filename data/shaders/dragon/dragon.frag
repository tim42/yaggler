

// test klmb fragment shader
// to see if yaggler works ;)

// Specify which version of GLSL we are using.

#version 330

#line 10

// #ifndef USING_KLMB_FRAMEWORK
// # error This shader must be used with the K:LMB framework
// #endif

uniform float global_time;
uniform vec2 screen_resolution;

uniform sampler2D texture;

in vec4  vertex_position;

#ifdef USING_KLMB_FRAMEWORK

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0);

in vec3 normal;
in vec3 tri_distance;


// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

#define BORDER          0.02


float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}

void KLMB_MAIN_FUNCTION()
{
  float d1 = min(min(tri_distance.x, tri_distance.y), tri_distance.z);
  d1 = amplify(d1, 10, 0.0);
  KLMB_SHARED_NAME(color_0).rgb = clamp(d1 , 0, 100) * (1 - vertex_position.z / 25.5) * (vec3(0.5, 0.9, 1.70)/*vertex_position.xyz*/) * length(normal.xy);
}

#else

out vec4 color_0;

// output something... _*red*_ :)
void main()
{
  color_0 = vec4(1., .0, .0, .0);
}

#endif


 
