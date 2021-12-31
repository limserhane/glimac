#version 330 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

out vec3 fColor;

uniform sampler2D uTexture;

void main() {
  fColor = texture(uTexture, vTexCoords).rgb;
};