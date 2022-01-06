#version 330 core

in vec3 vPosition_vs;
in vec3 vNormal;

out vec3 fColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

const vec3 ambient = vec3(0.01);

vec3 blinnPhong() {

  vec3 wi = normalize(uLightDir_vs);
  vec3 w0 = normalize(- vPosition_vs);

  vec3 normal = normalize(vNormal);

  vec3 halfway = normalize(uLightDir_vs + w0);

  vec3 diffuse = uKd * max(0.0, dot(wi, normal));
  vec3 specular = uKs * pow(max(0.0, dot(halfway, normal)), uShininess);

  return uLightIntensity * (diffuse + specular + ambient);
}

void main() {
  fColor = blinnPhong();
};