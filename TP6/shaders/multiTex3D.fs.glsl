#version 330 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

out vec3 fColor;

uniform sampler2D uSphereTexture;
uniform sampler2D uCloudTexture;

void main() {
  fColor = texture(uSphereTexture, vTexCoords).rgb + texture(uCloudTexture, vTexCoords).rgb;
};