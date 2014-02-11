
#version 330
#line 3


// // sanity checks
#ifndef USING_KLMB_FRAMEWORK
# error this file must be used with the k:lmb framework
#endif

#ifndef KLMB_IS_MAIN
# error this file must be the 'main' file in the k:lmb framework
#endif


// // std k:lmb shader framework macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT
// ok, this is the framework main
#define KLMB_FRAMEWORK_MAIN

// set the default number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER                    1


// // now this is some macro works :)

// declare output buffers
#define DECLARE_OUTPUT_BUFFER(n)                       out vec4 color_ ## n;
// declare shader output buffers
#define DECLARE_SHADER_OUTPUT_BUFFER(s, b)             vec4 KLMB_GET_PROG_SHARED(s, color_ ## b);
// declare shader main functions
#define DECLARE_SHADER_MAIN(n)                         void KLMB_CALL_MAIN_FUNCTION(n);


KLMB_FOR_EACH_OUTPUT_BUFFER(DECLARE_OUTPUT_BUFFER)
KLMB_FOR_EACH_SHADER_AND_OUTPUT_BUFFER(DECLARE_SHADER_OUTPUT_BUFFER)
KLMB_FOR_EACH_SHADER(DECLARE_SHADER_MAIN)


// // (or simply 'main')
void KLMB_MAIN_FUNCTION()
{
  // call every main functions
# define CALL_MAIN_FUNCTION(n)                           KLMB_CALL_MAIN_FUNCTION(n);
  KLMB_FOR_EACH_SHADER(CALL_MAIN_FUNCTION);

  // init buffers
# define INIT_OUTPUT_BUFFER(s, b)                     color_ ## b = vec4(0, 0, 0, 0);
  KLMB_FOR_EACH_SHADER_AND_OUTPUT_BUFFER(INIT_OUTPUT_BUFFER);

  // gather every values
# define GATHER_OUTPUT_BUFFER(s, b)                     color_ ## b += KLMB_GET_PROG_SHARED(s, color_ ## b);
  KLMB_FOR_EACH_SHADER_AND_OUTPUT_BUFFER(GATHER_OUTPUT_BUFFER);

  // reduce every values
# define REDUCE_OUTPUT_BUFFER(b)                        color_ ## b /= KLMB_TOTAL_PROG_NUMBER;
  KLMB_FOR_EACH_OUTPUT_BUFFER(REDUCE_OUTPUT_BUFFER);
}
