#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexCoords;

out vec2 vFragTexCoords;

uniform float uTime;

mat3 rotate(float a) {
  return mat3(
    vec3(cos(a), sin(a), 0),
    vec3(-sin(a), cos(a), 0),
    vec3(0, 0, 1)
  );
}

void main() {
  vFragTexCoords = aVertexTexCoords;
  gl_Position = vec4((rotate(uTime) * vec3(aVertexPosition, 1)).xy, 0, 1);
};