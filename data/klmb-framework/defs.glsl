
// standards defs for K:L:B shader framework.
// (included in each shader files that uses the framework).

//#define KLMB_PROG_ID		[0-9]+

// defined if the file is the 'main' file of the shader
//#define KLMB_IS_MAIN

#define USING_KLMB_FRAMEWORK

#ifdef KLMB_IS_MAIN
# define KLMB_SHARED_BASE_NAME

# define KLMB_MAIN_FUNCTION                     main
# define KLMB_SHARED_NAME(name)                 name

// something quite usefull :)
# define KLMB_CALL_MAIN_FUNCTION(x)             p ## x ## _klmb_main()
# define KLMB_GET_PROG_SHARED(x, name)          p ## x ## _klmb_ ## name


#else
# define _KLMB_SHARED_BASE_NAME(pid, name)      p ## pid ## _klmb_ ## name

// nvidia...
# define _KLMB_MAIN_FUNCTION(x)                 _KLMB_SHARED_BASE_NAME(x, main)
# define _KLMB_SHARED_NAME(x, name)             _KLMB_SHARED_BASE_NAME(x, name)

# define KLMB_MAIN_FUNCTION                     _KLMB_MAIN_FUNCTION(KLMB_PROG_ID)
# define KLMB_SHARED_NAME(name)                 _KLMB_SHARED_NAME(KLMB_PROG_ID, name)

#endif


