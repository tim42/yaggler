
#version 330


layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in gl_PerVertex
{
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];

out vec3 normal;
out vec4 vertex_position;

out vec3 tri_distance;

void main()
{
  normal = normalize(cross((gl_in[0].gl_Position - gl_in[2].gl_Position).xyz, (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz));

  gl_Position = gl_in[0].gl_Position;
  tri_distance = vec3(1, 0, 0);
  vertex_position = gl_Position;
  EmitVertex();

  gl_Position = gl_in[1].gl_Position;
  tri_distance = vec3(0, 1, 0);
  vertex_position = gl_Position;
  EmitVertex();

  gl_Position = gl_in[2].gl_Position;
  tri_distance = vec3(0, 0, 1);
  vertex_position = gl_Position;
  EmitVertex();


  EndPrimitive();
}
