
#version 330 core

#line 4



#ifdef USING_KLMB_FRAMEWORK

// from the geom stage
in vec3 gbuffer_normal;
in vec4 gbuffer_position;

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            2

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0); // color          (rgba)
KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_1); // normal + depth (rgb + a)

// color_0 is computed by the other shader, color_1 is computed by this one.

void KLMB_MAIN_FUNCTION()
{
  KLMB_SHARED_NAME(color_1).rgb = gbuffer_normal;
  KLMB_SHARED_NAME(color_1).a = gbuffer_position.z; // .w ?
}

#else

out vec4 color_0;

// output something... _*red*_ :)
void main()
{
  color_0 = vec4(1., .0, .0, .0);
}

#endif
