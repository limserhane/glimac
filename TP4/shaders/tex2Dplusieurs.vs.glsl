#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexCoords;

out vec2 vFragTexCoords;
out vec3 vColor;

uniform mat3 uModelMatrix;
uniform vec3 uColor;

void main() {
  vFragTexCoords = aVertexTexCoords;
  vColor = uColor;
  gl_Position = vec4((uModelMatrix * vec3(aVertexPosition, 1)).xy, 0, 1);
};