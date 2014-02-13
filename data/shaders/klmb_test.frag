

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

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

void KLMB_MAIN_FUNCTION()
{
  vec2 frag_coord = (vertex_position.xy / 2 + 0.5) * screen_resolution;
  vec4 tex_color = texture2D(texture, vec2(frag_coord.xy / screen_resolution.xy));

  vec2 n_coord = gl_FragCoord.xy / (screen_resolution.xy * vec2(0.5, 0.5)) + 0.9;
  vec2 r_coord = n_coord * screen_resolution.xy;
  float stime = abs(cos(global_time / 5 + r_coord.x + r_coord.y) * 5 + sin(global_time / 5) * 15) + 20;
  KLMB_SHARED_NAME(color_0) = (vec4(0.5 * abs(cos(stime * n_coord.y * n_coord.x)) * 0.25 * abs(sin(stime / n_coord.x* n_coord.y * 10)), 0., 0., 0.));
  KLMB_SHARED_NAME(color_0) = KLMB_SHARED_NAME(color_0).xxxw * 8. - vec4((tex_color.rgb * tex_color.a).rgb, 1.);
  KLMB_SHARED_NAME(color_0) = KLMB_SHARED_NAME(color_0).xxxw * 4. * vec4((tex_color.rgb * tex_color.a).rgb, 1.);
}

#else

out vec4 color_0;

// output something... _*red*_ :)
void main()
{
  color_0 = vec4(1., .0, .0, .0);
}

#endif


