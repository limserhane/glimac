#version 330 core

in vec2 vFragPosition;

out vec3 fFragColor;

vec2 complexSquare(vec2 z) {
  return vec2(z.x * z.x - z.y * z.y, 2 * z.x * z.y);
}

void main() {

  int N = 5000;
  vec2 c = vFragPosition;
  
  vec2 zk = vec2(0.0);
  float mandelbrot = 1.0;

  for(int k = 0; k < N; k++) {

    zk = complexSquare(zk) + c;

    if(length(zk) > 2.0) {
      mandelbrot = k/N;
      break;
    }
  }

  vec3 color = vec3((1.0 + vFragPosition.x) / 2.0, 0.0, (1.0 + vFragPosition.y) / 2.0);
  fFragColor = mandelbrot * color;
};