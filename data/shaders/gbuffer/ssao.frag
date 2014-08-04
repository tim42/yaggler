
#version 330 core

#line 4

// original sources found at Game Developer Magazine March 2008
// from Ogre's Crease Shading sample (in CG)
// rewritten in GLSL for Ogre by tim. (2013)
// rewritten in K:LMB / GLSL for YägGLer by tim. (2014)


#ifdef USING_KLMB_FRAMEWORK

// // some mandatory K:LMB macros

// this file is using the 'framework'
#define KLMB_IS_USING_FRAMEWORK
// this file is a shader entry point
#define KLMB_IS_ENTRY_POINT

// the number of output buffers
#define KLMB_NUMBER_OF_OUTPUT_BUFFER            1

KLMB_OUTPUT_VAR vec4 KLMB_SHARED_NAME(color_0); // color                (rgba)


uniform sampler2D scene;                        // color                (rgba)
uniform sampler2D geometry;                     // normal + depth       (rgb + a)

uniform mat4 cam_view;
uniform mat4 cam_vp;

uniform vec2 buffer_size;                       // screen size

uniform float cRange = 1.0; // the three(four) autistic (and not artistic at all) parameters
uniform float cBias = 1.7;
uniform float cAverager = 25.0;
uniform float cMinimumCrease = 0.1;
uniform float cKernelSize = 2.5; // Bias for the kernel size, HACK for the fixed size 11x11 stipple kernel



vec3 coordtopos(vec2 coord, float depth, vec2 inv_buffer_size)
{
  vec3 screencoord = vec3(((coord.x * inv_buffer_size.x) - 0.5) * 2.0 * (buffer_size.x * inv_buffer_size.y), ((-coord.y * inv_buffer_size.y) + 0.5) * 2.0 / (buffer_size.x * inv_buffer_size.y), (depth));
  screencoord.x *= -screencoord.z;
  screencoord.y *= screencoord.z;
  return (vec4(-screencoord, 0) * cam_view).xyz;
}

void KLMB_MAIN_FUNCTION()
{
    vec2 inv_buffer_size = 1. / buffer_size;
    vec2 uv = gl_FragCoord.xy * inv_buffer_size;

    // get the view space position and normal of the fragment
    vec4 geom = texture(geometry, uv);
    vec3 fragmentNormal = -geom.xyz;

    float distance = geom.w;
    vec3 fragmentPosition = (vec4(coordtopos(gl_FragCoord.xy, distance, inv_buffer_size), 0) ).xyz;


    float totalGI = 0.0;

    // a diamond shaped (45deg rotated square) stipple pattern around (0, 0) this will be used as offset for the samples
    //        O
    //       O O
    //      O O O
    //     O O O O
    //    O O X O O
    //     O O O O
    //      O O O
    //       O O
    //        O
    // the algorith starts with the leftmost element and walks the diagonal to the topmost element
    // a stippleSize of n yields (((n - 1) / 2)^2) - 1 samples
    // the 'image' above has a stipple size of 11 'cuz it has 5 samples (minus
    // the current fragment position and 4 gaps = 11.

    const int stippleSize = 11; // must be odd
    for (int i = 0; i < (stippleSize + 1) / 2; i++)
    {
        vec2 diagonalStart = vec2(-(stippleSize - 1) / 2, 0) + i;
        for(int j = 0; j < (stippleSize + 1) / 2; j++)
        {
            vec2 sampleOffset = diagonalStart + vec2(j, -j);

            vec2 sampleUV = uv + (sampleOffset * inv_buffer_size.xy * cKernelSize);
            float depth = texture(geometry, sampleUV).w;
            vec3 samplePos = coordtopos(sampleOffset * cKernelSize + gl_FragCoord.xy, depth, inv_buffer_size);

            vec3 toCenter = (samplePos - fragmentPosition);
            float distance = length(toCenter);

            toCenter = normalize(toCenter);
            float centerContrib = clamp(((dot(toCenter, ((fragmentNormal)))) - cMinimumCrease) * cBias, 0., 1.);
            float rangeAttenuation = 1.0f - clamp(distance / cRange, 0., 1.);

            totalGI += centerContrib * rangeAttenuation;
        }
    }

    totalGI /= cAverager;

    // remove OC where the scene is well lit (next lights)
//     vec3 scene_color = texture(sColor, uv).rgb;
   vec4 lit_scene_color = texture(scene, uv);

   float factor = 1.0 - length(abs(lit_scene_color/* - scene_color*/));

   totalGI *= clamp(factor + .5, .0, 1.); // .5

   /*vec4(geom.xyz, 1) ;*//* 0.5 + 0.5;*/
   KLMB_SHARED_NAME(color_0) = (lit_scene_color - (vec4(vec3(totalGI), 0)));
//    KLMB_SHARED_NAME(color_0) = 1- vec4(vec3(totalGI), 0.);
}

#endif