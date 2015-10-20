
#version 330 core

#line 4

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

// create the selection effect (darken the scene)
KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0);

uniform float selection_factor = 1.;
// uniform float global_time;

void KLMB_MAIN_FUNCTION()
{
  KLMB_SHARED_NAME(color_0).a = selection_factor;
}
