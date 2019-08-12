#version 330 core
#ifdef NOVA_VERTEX_SHADER
layout(location = 0) in vec3 vertexPosition;
uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main() { gl_Position = lightSpaceMatrix * model * vec4(vertexPosition, 1.0); }

#endif
#ifdef NOVA_FRAGMENT_SHADER

void main() {}
#endif