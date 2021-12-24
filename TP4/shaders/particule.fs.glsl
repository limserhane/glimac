#version 330 core

in vec3 vFragColor;
in vec2 vFragPosition;

out vec3 fFragColor;

void main() {
  float alpha = 1.5;
  float beta = 100.0;

  float d = distance(vec2(0, 0), vFragPosition);
  float a = alpha * exp(-beta * d * d);
  fFragColor = a * vFragColor;
};