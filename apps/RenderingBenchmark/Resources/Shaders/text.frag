#version 330

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D text;
uniform vec4 color;

void main() {
  fragColor = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r) * color;
}