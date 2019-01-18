#version 330 core
#ifdef NOVA_VERTEX_SHADER

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 textureCoordinates;
out vec2 lerpedTexCoords;

void main()
{
    gl_Position = vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0);
    lerpedTexCoords = textureCoordinates;
}

#endif
#ifdef NOVA_FRAGMENT_SHADER

in vec2 lerpedTexCoords;
out vec4 fragment;

uniform sampler2D uGPosMetal;
uniform sampler2D uGNormRough;
uniform sampler2D uGAlbedoSkyboxmask;
uniform sampler2D uGNormalMapAO;
uniform samplerCube uIrradianceMap;
uniform samplerCube uRadianceMap;
uniform sampler2D uBRDFLUT;

struct Light
{
    vec3 position;
    vec3 color;
    int type;
};

uniform int uCurrentMaxLights;
uniform Light uLights[MAX_LIGHTS];
uniform vec3 uCameraPos;
// uniform mat4 view;

const float PI = 3.14159265359;
const float MAX_REFLECTION_LOD = 4.0;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
vec3 getNormalFromMap();

vec3 gamma(vec3 color)
{
    return pow(color,vec3(1.0/2.2));
}

vec3 deGamma(vec3 color)
{
    return pow(color,vec3(2.2));
}
vec4 gamma(vec4 color)
{
    return pow(color,vec4(1.0/2.2));
}

vec4 deGamma(vec4 color)
{
    return pow(color,vec4(2.2));
}
float deGamma(float color)
{
    return pow(color,2.2);
}
void main()
{    
    // retrieve data from G-buffer
    vec4 temp;
    temp = texture(uGPosMetal, lerpedTexCoords);
    //temp = deGamma(temp);
    //temp.a = deGamma(temp.a);
    vec3 WorldPos = temp.rgb;
    float metallic = temp.a;

    temp = texture(uGNormRough, lerpedTexCoords);
    //temp = deGamma(temp);
    //temp.a = deGamma(temp.a);
    vec3 Normal = temp.rgb;
    float roughness = temp.a;

    temp = texture(uGAlbedoSkyboxmask, lerpedTexCoords);    
    vec3 albedo = temp.rgb;
    //albedo  = deGamma(albedo);
    float skyboxMask = temp.a;

    temp = texture(uGNormalMapAO, lerpedTexCoords);
    //temp = deGamma(temp);
    vec3 normalMap = temp.rgb;
    float ao = temp.a;

    // input lighting data
    // vec3 N = getNormalFromMap();

    vec3 tangentNormal = normalMap * 2.0 - 1.0;

    vec3 Q1 = dFdx(WorldPos);
    vec3 Q2 = dFdy(WorldPos);
    vec2 st1 = dFdx(lerpedTexCoords);
    vec2 st2 = dFdy(lerpedTexCoords);

    vec3 N = normalize(Normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    N = normalize(TBN * tangentNormal);

    vec3 V = normalize(uCameraPos - WorldPos);
    vec3 R = reflect(-V, N);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < uCurrentMaxLights; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(uLights[i].position - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(uLights[i].position - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = uLights[i].color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 nominator = NDF * G * F;
        float NdotL = max(dot(N, L), 0.0);
        float denominator =
            4 * max(dot(N, V), 0.0) * NdotL + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;

        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance *
              NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't
                     // multiply by kS again
    }

    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(uIrradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;

    // sample both the pre-filter map and the BRDF lut and combine them together as per the
    // Split-Sum approximation to get the IBL specular part.
    vec3 prefilteredColor = textureLod(uRadianceMap, R, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(uBRDFLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = ambient + Lo;

    // float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    // float brightness = (color.r+color.r+color.b+color.g+color.g+color.g)/6; //aproximation
    // float brightness =
    // (color.r+color.r+color.r+color.b+color.g+color.g+color.g+color.g)/8;//another aproximation
    // if(brightness < 1.0) color = vec3(0.0);
    // color = color*brightness-color;
    // color =max (albedo * skyboxMask, pow(color,vec3(2.2)));//so it doesnt light the skybox kinda
    // ulgy..


    //color = max(albedo * skyboxMask, color); // so it doesnt light the skybox kinda ulgy..

    // gamma correct
    //color = pow(color, vec3(1.0/2.2));        

    fragment = vec4(color, 1.0);
}

// Trowbridge-Reitz GGX normal distribution function
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    // F0 = surface reflection at zero incidence
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}
vec3 getNormalFromMap()
{
    // vec3 tangentNormal = texture(normalMap, lerpedTexCoords).xyz * 2.0 - 1.0;
    /*vec3 tangentNormal = normalMap.xyz * 2.0 - 1.0;

vec3 Q1  = dFdx(WorldPos);
vec3 Q2  = dFdy(WorldPos);
vec2 st1 = dFdx(lerpedTexCoords);
vec2 st2 = dFdy(lerpedTexCoords);

vec3 N   = normalize(Normal);
vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
vec3 B  = -normalize(cross(N, T));
mat3 TBN = mat3(T, B, N);

return normalize(TBN * tangentNormal);*/
    return vec3(1);
}
#endif