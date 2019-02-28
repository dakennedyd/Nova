#version 330 core
#ifdef NOVA_VERTEX_SHADER

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 norm;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uNormalMat;

out vec2 lerpedTexCoords;
out vec3 fragPos;
//out vec3 normal;

void main()
{
    lerpedTexCoords = texCoords;
    fragPos = vec3(uModel * vec4(vertPos, 1.0));
    //normal = vec3(uModel * vec4(norm, 0.0f));
    float size = 1.0;
    vec3 cameraRight_worldspace = vec3(uView[0][0], uView[1][0], uView[2][0]);
    vec3 cameraUp_worldspace = vec3(uView[0][1], uView[1][1], uView[2][1]);
    vec3 vertexPosition_worldspace =
    vertPos
    + cameraRight_worldspace * -0.5 * size
    + cameraUp_worldspace *  0.5 * size;

    //mat4 NormalMat = mat4(transpose(inverse(mat3(uModel))));
    //normal = normalize(vec3(NormalMat * vec4(norm, 0.0)));
    vec4 pos = uProj * uView * vec4(fragPos, 1.0); // uModel * vec4 (vertPos, 1.0);    
    //fragPos = vec3(pos);
    gl_Position = vec4(vertexPosition_worldspace,1.0);
}
#endif
#ifdef NOVA_FRAGMENT_SHADER

in vec2 lerpedTexCoords;
in vec3 fragPos;
//in vec3 normal;

// uniform sampler2D diffuse0;
// material parameters
uniform sampler2D uAlbedoMap;
// uniform sampler2D metallicMap;
// uniform sampler2D roughnessMap;
// uniform sampler2D aoMap;
//uniform sampler2D uDetailsMap; // RED CHANNEL = metalness GREEN CHANNEL = roughness BLUE CHANNEL =
                               // ambient occlusion
//uniform sampler2D uNormalMap;

layout(location = 0) out vec4 gPosMetal;
layout(location = 1) out vec4 gNormRough;
layout(location = 2) out vec4 gAlbedoSkyboxmask;
layout(location = 3) out vec4 gNormalMapAO;

void main()
{
    // vec4 details = texture(uDetailsMap, lerpedTexCoords);
    // //details = pow(details, vec4(2.2));
    // // store the fragment position vector in the first gbuffer texture
    //gPosMetal = vec4(fragPos, 0.0);
    // // also store the per-fragment normals into the gbuffer
    // gNormRough = vec4(normalize(normal), details.g);
    // // and the diffuse per-fragment color
    gPosMetal = vec4(1.0);
    gNormRough= vec4(1.0);
    gAlbedoSkyboxmask.rgb = texture(uAlbedoMap, lerpedTexCoords).rgb;
    //gAlbedoSkyboxmask.rgb = pow(gAlbedoSkyboxmask.rgb, vec3(2.2));
    gAlbedoSkyboxmask.a = 0; // details.b;//texture(diffuse0, lerpedTexCoords).r;
    gNormalMapAO= vec4(1.0);
    
    // store specular intensity in gAlbedoSpec's alpha component
    //gNormalMapAO.xyz = texture(uNormalMap, lerpedTexCoords).xyz;    
    //gNormalMapAO.xyz = pow(gNormalMapAO.xyz, vec3(2.2));

    //gNormalMapAO.w = details.b;
    //gNormalMapAO.w = pow(gNormalMapAO.w, 2.2);
}
#endif