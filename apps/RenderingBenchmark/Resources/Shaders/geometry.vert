#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in mat4 instanceMatrix;
layout (location = 5) in vec4 instanceColor;

out mat4 mvp;
out vec4 vColor;

void main()
{
  gl_Position = vec4(pos, 1.0);
  mvp = instanceMatrix;
  vColor = instanceColor;
}