#version 330 core
#ifdef NOVA_VERTEX_SHADER

layout (location = 0) in vec3 vertPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform mat4 normalMat;

out vec3 fragPos;
out vec3 normal;
out vec3 lerpedTexCoords;

void main () {	
	lerpedTexCoords = vertPos;
	fragPos = vec3(model * vec4(vertPos, 1.0));	
	normal = normalize(vec3(normalMat * vec4(vertPos, 0.0)));  
	gl_Position = proj * view * model * vec4 (vertPos, 1.0);
}
#endif
#ifdef NOVA_FRAGMENT_SHADER

//Fragment Shader with Cube Texture
in vec3 lerpedTexCoords;
in vec3 normal;
in vec3 fragPos;

uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec4 materialSpecular;    

struct Light {
	int type;
    vec3 position;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

uniform Light light[MAX_LIGHTS];  
uniform mat4 view;
uniform samplerCube diffuse0;


layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

void main () 
{	
	// store the fragment position vector in the first gbuffer texture
    gPosition = vec4(fragPos,1);
    // also store the per-fragment normals into the gbuffer
    gNormal = vec4(normalize(normal),1);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(diffuse0, lerpedTexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = 1;//texture(diffuse0, lerpedTexCoords).r;
}
#endif