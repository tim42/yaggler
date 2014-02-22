
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

uniform vec2 direction;                 // blur direction
uniform vec2 buffer_size;               // screen size

uniform float center = 9.;              // the focus point (z-distance
uniform float max_distance = 15.;       // the minimal distance from 'center' where the blur will be maximal.

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0); // color          (rgba)
//KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_1); // normal + depth (rgb + a)

// the max samples
#define NUM_BLUR_SAMPLES 10.0

// gaussian distrib'
#define SQUARE(x)       ((x) * (x))
#define GAUSS_DISTRIB   (sqrt(-SQUARE(NUM_BLUR_SAMPLES + 1.0f) / -11.082527f))
#define GAUSS(x)        (1.0 / (2.506628275 * GAUSS_DISTRIB) * exp(-(SQUARE(x) / (2.0 * SQUARE(GAUSS_DISTRIB)))))

// distance where the Blur is maximum
#define MAX_DST 15.f

void KLMB_MAIN_FUNCTION()
{
  // direction factor
  vec2 o = vec2(direction.x / buffer_size.x, direction.y / buffer_size.y);

  vec2 uv = gl_FragCoord.xy / buffer_size;

  // blur
  float denom = 0.1;
  vec4 sum = texture(scene, uv) * denom;

  /*float center = 9.;*//*texture(geometry, vec2(0.5, 0.5)).w
                + texture(geometry, vec2(0.5 + 0.02, 0.5 + 0.00)).w
                + texture(geometry, vec2(0.5 - 0.02, 0.5 + 0.00)).w
                + texture(geometry, vec2(0.5 + 0.00, 0.5 + 0.02)).w
                + texture(geometry, vec2(0.5 + 0.00, 0.5 - 0.02)).w

                + texture(geometry, vec2(0.5 + 0.02, 0.5 + 0.02)).w
                + texture(geometry, vec2(0.5 - 0.02, 0.5 + 0.02)).w
                + texture(geometry, vec2(0.5 + 0.02, 0.5 - 0.02)).w
                + texture(geometry, vec2(0.5 - 0.02, 0.5 - 0.02)).w;
  center /= 9.;*/

  float alpha = texture(geometry, uv).w ;
    if (alpha == 0)
      alpha = 10000.;
  alpha = abs(center - alpha);
  alpha = clamp(alpha, 0, max_distance) / max_distance;

  for (float i = 0; i < NUM_BLUR_SAMPLES; ++i)
  {
    float idx = i - (NUM_BLUR_SAMPLES / 2.0);
    vec2 nuv = uv + o * (idx * 2.0);


    float coef = GAUSS(idx);
    sum += texture(scene, nuv) * coef * alpha;
    denom += coef * alpha;
  }

  KLMB_SHARED_NAME(color_0) = vec4((sum / denom).xyzw);
}

#endif
