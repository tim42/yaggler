
#version 330
#line 3


// // sanity checks
#ifndef USING_KLMB_FRAMEWORK
# error this file must be used with the k:lmb framework
#endif

// test if the shader is the one with the main() function
// (this won't be the case in shaders with only one fragment/...)
#ifdef KLMB_IS_MAIN

// // std k:lmb shader framework macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT
// ok, this is the framework main
#define KLMB_FRAMEWORK_MAIN



// main
void KLMB_MAIN_FUNCTION()
{
  
}

#else

// shader does not declare anything: fails on intelHD/mesa
// this var is unused.
bool KLMB_DO_NOT_TOUCH_THIS_VAR_1_23456213613641468454212121545545451684118648;

#endif