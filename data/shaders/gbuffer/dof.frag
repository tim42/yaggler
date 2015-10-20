
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

uniform float center = 40.0;              // the focus point (z-distance
uniform float max_distance = 9.;       // the minimal distance from 'center' where the blur will be maximal.
uniform float min_distance = 0.;       // the maximal distance from 'center' where the blur will be null.

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0); // color          (rgba)
// KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_1); // normal + depth (rgb + a)

// the max samples
#define NUM_BLUR_SAMPLES 30.0

// gaussian distrib'
#define SQUARE(x)       ((x) * (x))
#define GAUSS_DISTRIB   (sqrt(-SQUARE(NUM_BLUR_SAMPLES + 1.0f) / -11.082527f))
#define GAUSS(x)        (1.0 / (2.506628275 * GAUSS_DISTRIB) * exp(-(SQUARE(x) / (2.0 * SQUARE(GAUSS_DISTRIB)))))

void KLMB_MAIN_FUNCTION()
{
  // direction factor
  vec2 o = vec2(direction.x / buffer_size.x, direction.y / buffer_size.y);

  vec2 uv = gl_FragCoord.xy / buffer_size;

  // blur
  float denom = 0.1;
  vec4 geom = texture(geometry, uv);
  vec4 sum = texture(scene, uv) * denom;
  vec4 geom_sum = geom * denom;



  float alpha = geom.w ;
  alpha = (clamp(abs(center - alpha), min_distance, max_distance) - min_distance) / (max_distance - min_distance);
  if (geom.w == 0.)
    alpha = 1.;


  for (float i = 0.; i < NUM_BLUR_SAMPLES * alpha * alpha * alpha; ++i)
  {
    float idx = i - (NUM_BLUR_SAMPLES * alpha * alpha * alpha / 2.0);
    vec2 nuv = uv + o * (idx * 1.0);

    float coef = GAUSS(idx);
    vec4 color = texture(scene, nuv);
    float depth = texture(geometry, nuv).w;
    if (depth == 0)
      depth = alpha;
    else
      depth = (clamp(abs(center - depth), min_distance, max_distance) - min_distance) / (max_distance - min_distance);

    float k = (1. + 15. * length(color.xyz) * cos(i / (NUM_BLUR_SAMPLES /** alpha*/) * (3.1415 / 6.)));
//     float d = clamp((depth - alpha + 0.9) * depth, 0, 0.9) * 2.0;
    float diff = abs(alpha - depth);
    float d = (1. - sqrt(diff) / k) * depth * depth * 2. * coef * alpha * alpha * (/*sqrt*/((k)));
//     sum += color * coef * alpha * (sqrt((k))) * d;
    sum += color * d;
    denom += d;
  }

  KLMB_SHARED_NAME(color_0) = vec4((sum / denom).xyzw);
}

#endif
