
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


// // now this is some macro works :)

// declare shader main functions
#define DECLARE_SHADER_MAIN(n)                         void KLMB_CALL_MAIN_FUNCTION(n);


KLMB_FOR_EACH_SHADER(DECLARE_SHADER_MAIN)


void KLMB_MAIN_FUNCTION()
{
  // call every main functions
# define CALL_MAIN_FUNCTION(n)                           KLMB_CALL_MAIN_FUNCTION(n);
  KLMB_FOR_EACH_SHADER(CALL_MAIN_FUNCTION);
}

#else

// shader does not declare anything, fails on intelHD/mesa
// this var is aimed to be unused.
bool KLMB_DO_NOT_TOUCH_THIS_VAR_1_23456213613641364136841651941355541684118648;

#endif
