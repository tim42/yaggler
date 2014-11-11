
#version 330 core

#line 4



#ifdef USING_KLMB_FRAMEWORK

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

uniform sampler2D scene;                // color                (rgba)
uniform sampler2D geometry;             // normal + depth       (rgb + a)

uniform float global_time;
uniform vec2 buffer_size;               // screen size

uniform float flash_time;
uniform vec4 flash_color;

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0); // color          (rgba)
// KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_1); // normal + depth (rgb + a)



void KLMB_MAIN_FUNCTION()
{
  vec2 uv = gl_FragCoord.xy / buffer_size;
  vec4 scene_color = texture(scene, uv);
  vec4 geom = texture(geometry, uv);

  // a very simple flash equation:
  vec3 flash = abs(sin(1./exp(abs(global_time - flash_time) * 2.5) + 2. * 3.1415)) * flash_color.rgb;

  // result:
  // (also add the selection effect)
  KLMB_SHARED_NAME(color_0).rgb = (scene_color.rgb) * scene_color.a + abs(flash);
}


#endif
