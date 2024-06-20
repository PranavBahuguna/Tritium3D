#version 430 core

layout (location = 0) uniform vec3 pos;
layout (location = 1) in mat4 instanceModel;
layout (location = 5) in vec4 instanceColor;

uniform mat4 projectionView;

out mat4 mvp;
out vec4 vColor;

void main()
{
  gl_Position = vec4(pos, 1.0);
  mvp = projectionView * instanceModel;
  vColor = instanceColor;
}