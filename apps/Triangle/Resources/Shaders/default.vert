#version 330 core

layout (location = 0) in vec2 aPos;

uniform mat4 modelProjection;

void main()
{
    gl_Position = modelProjection * vec4(aPos.xy, 0.0, 1.0);
}