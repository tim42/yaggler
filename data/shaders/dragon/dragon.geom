
#version 330


layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in gl_PerVertex
{
    vec4 gl_Position;
} gl_in[];

out gl_PerVertex
{
    vec4 gl_Position;
};


out vec3 normal;
out vec4 vertex_position;

out vec3 tri_distance;

uniform float global_time;

void main()
{

  normal = normalize(cross((gl_in[2].gl_Position - gl_in[0].gl_Position).xyz, (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz));

  vec4 n = 0.01 / (vec4((normal), 1.0)) * abs(1. + sin((global_time / 5. - length(gl_in[0].gl_Position) / 3.5))) * 3.1;


  gl_Position = gl_in[0].gl_Position + n;
  tri_distance = vec3(1, 0, 0);
  vertex_position = gl_Position;
  EmitVertex();

  gl_Position = gl_in[1].gl_Position + n;
  tri_distance = vec3(0, 1, 0);
  vertex_position = gl_Position;
  EmitVertex();

  gl_Position = gl_in[2].gl_Position + n;
  tri_distance = vec3(0, 0, 1);
  vertex_position = gl_Position;
  EmitVertex();


  EndPrimitive();
}
