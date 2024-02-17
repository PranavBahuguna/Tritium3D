#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in mat4 instanceMatrix;
layout (location = 5) in vec4 instanceColor;

uniform mat4 projectionView;

out vec4 vertexColor;

void main()
{
  gl_Position = projectionView * instanceMatrix * vec4(pos, 1.0);
  vertexColor = instanceColor;
}