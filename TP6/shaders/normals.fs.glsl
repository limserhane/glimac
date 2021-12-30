#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

out vec3 fColor;

void main() {
    fColor = normalize(vNormal);
}