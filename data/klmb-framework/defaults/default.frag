

// test klmb fragment shader
// to see if yaggler works ;)

// Specify which version of GLSL we are using.

#version 330 core

#line 10


#ifdef USING_KLMB_FRAMEWORK


// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0);

void KLMB_MAIN_FUNCTION()
{
  KLMB_SHARED_NAME(color_0) = vec4(0., .68, 1., 1.);
}

#else

out vec4 color_0;

// output something... _*red*_ :)
void main()
{
  color_0 = vec4(1., .0, .0, 1.);
}

#endif
