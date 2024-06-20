#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 128) out;

uniform int nSides;

in mat4 mvp[];
in vec4 vColor[];

out vec4 fColor;

const float twoPi = 6.28318530718;

void main() {
  fColor = vColor[0];
  vec4 centerPos = mvp[0] * gl_in[0].gl_Position;

  float anglePerSide = twoPi / nSides;
  float sinA = sin(anglePerSide);
  float cosA = cos(anglePerSide);
  float curX = 1;
  float curY = 0;

  for (int i = 0; i <= nSides; i++) {
    // Generate circle center vertex
    gl_Position = centerPos;
    EmitVertex();
    
    // Generate circle edge vertex
    float nCurX = cosA * curX - sinA * curY;
    curY =        sinA * curX + cosA * curY;
    curX = nCurX;
    vec4 offset = mvp[0] * vec4(curX, curY, 0.0, 0.0);
    gl_Position = centerPos + offset; 
    EmitVertex();
  }
  EndPrimitive();
}