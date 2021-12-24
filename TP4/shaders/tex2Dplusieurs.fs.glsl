#version 330 core

in vec2 vFragTexCoords;
in vec3 vColor;

out vec3 fFragColor;

void main() {
  fFragColor = vColor;
};