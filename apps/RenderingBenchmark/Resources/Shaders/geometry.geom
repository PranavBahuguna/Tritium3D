#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 projectionView;

in mat4 mvp[];
in vec4 vColor[];

out vec4 gColor;

void main() {
  vec4 pos = gl_in[0].gl_Position;
  gColor = vColor[0];

  gl_Position = projectionView * mvp[0] * (pos + vec4(-0.5, -0.5, 0.0, 0.0)); // bottom-left
  EmitVertex();   
  gl_Position = projectionView * mvp[0] * (pos + vec4(0.5, -0.5, 0.0, 0.0));  // bottom-right
  EmitVertex();
  gl_Position = projectionView * mvp[0] * (pos + vec4(-0.5, 0.5, 0.0, 0.0));  // top-left
  EmitVertex();
  gl_Position = projectionView * mvp[0] * (pos + vec4(0.5, 0.5, 0.0, 0.0));   // top-right
  EmitVertex();
  EndPrimitive();
} 