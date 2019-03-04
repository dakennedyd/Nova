#version 330 core
#ifdef NOVA_VERTEX_SHADER

layout(location = 0) in vec3 vertexPosition;
// layout (location = 1) in vec3 texCoords;

// uniform mat4 model;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 lerpedTexCoords;

void main()
{
    lerpedTexCoords = vertexPosition;
    // vec4 pos =  uProj * uView * model * vec4(vertPos, 1.0);
    vec4 pos = uProj * mat4(mat3(uView)) * vec4(vertexPosition, 1.0);
    gl_Position = pos.xyww;
    //gl_Position = pos;

    // gl_Position = uProj * uView * vec4 (vertPos, 1.0);
    // gl_Position = vec4(pos.xy, 1.0, 1.0);
    // gl_Position = pos;
    // gl_Position = vec4(vertexPosition.x, vertexPosition.y, 1.0, 1.0);
}

#endif
#ifdef NOVA_FRAGMENT_SHADER

in vec3 lerpedTexCoords;

// uniform vec3 color;
// uniform vec3 lightColor;

uniform samplerCube uSkyboxTexture;

//layout(location = 2) out vec4 gAlbedoSkyboxmask;
out vec4 gAlbedoSkyboxmask;

void main()
{

    // fragment = vec4(1.0, 1.0, 0.0, 1.0);
    // fragment = vec4(texture(albedoMap, lerpedTexCoords).xyz, 1.0);
    //if(gl_FragCoord.z < -1.) discard;
    gAlbedoSkyboxmask = texture(uSkyboxTexture, lerpedTexCoords);
    //gAlbedoSkyboxmask.rgb = pow(gAlbedoSkyboxmask.rgb,vec3(2.2));
    //gAlbedoSkyboxmask.rgb = pow(gAlbedoSkyboxmask.rgb,vec3(1.0/2.2));
    gAlbedoSkyboxmask.a = 1.0;
}
#endif