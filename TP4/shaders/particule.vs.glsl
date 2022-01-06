#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;

out vec2 vFragPosition;

mat3 translate(float tx, float ty) {
  mat3 M = mat3(1);
  M[2].x = tx;
  M[2].y = ty;
  return M;
}

mat3 scale(float sx, float sy) {
  mat3 M = mat3(1);
  M[0].x = sx;
  M[1].y = sy;
  return M;
}

mat3 rotate(float a) {
  return mat3(
    vec3(cos(a), sin(a), 0),
    vec3(-sin(a), cos(a), 0),
    vec3(0, 0, 1)
  );
}

void main() {
  vFragColor = aVertexColor;
  
  vec2 transformed = ( rotate(-90) * scale(2, 2) * vec3(aVertexPosition, 1)).xy;
  gl_Position = vec4(transformed, 0, 1);

  vFragPosition = aVertexPosition;
};