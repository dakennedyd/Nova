#version 330 core
#ifdef NOVA_VERTEX_SHADER

layout (location = 0) in vec3 vertPos;

uniform mat4 uView;
uniform mat4 uProj;

void main () {		
    gl_Position = uProj * uView * vec4 (vertPos, 1.0);
}
#endif
#ifdef NOVA_FRAGMENT_SHADER

uniform vec3 uColor;
out vec4 fragment;

void main () 
{
    fragment = vec4(uColor, 1.0);
}
#endif