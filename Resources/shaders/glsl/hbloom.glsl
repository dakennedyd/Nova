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
uniform sampler2D uAlbedo;
out vec4 fragment;

mat4 contrastMatrix( float contrast )
{
	float t = ( 1.0 - contrast ) / 2.0;
    
    return mat4( contrast, 0, 0, 0,
                 0, contrast, 0, 0,
                 0, 0, contrast, 0,
                 t, t, t, 1 );

}

mat4 saturationMatrix( float saturation )
{
    vec3 luminance = vec3( 0.3086, 0.6094, 0.0820 );
    
    float oneMinusSat = 1.0 - saturation;
    
    vec3 red = vec3( luminance.x * oneMinusSat );
    red += vec3( saturation, 0, 0 );
    
    vec3 green = vec3( luminance.y * oneMinusSat );
    green += vec3( 0, saturation, 0 );
    
    vec3 blue = vec3( luminance.z * oneMinusSat );
    blue += vec3( 0, 0, saturation );
    
    return mat4( red,     0,
                 green,   0,
                 blue,    0,
                 0, 0, 0, 1 );
}

const float offset = 1.0 / 300.0;
void main()
{
    const int lod = 0;

    float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
    vec2 tex_offset = 1.0 / textureSize(uAlbedo, lod) * 2.0; // gets size of single texel
    vec4 result = textureLod(uAlbedo, lerpedTexCoords, lod)
                //* .25 * contrastMatrix(5.) * saturationMatrix(1.1)
                * weight[0]; // current fragment's contribution
    // vec3 result = texture(uAlbedo, lerpedTexCoords).rgb * weight[0]; // current fragment's
    // contribution for(int i2 = 0; i2 < 5; ++i2)
    for (int i = 1; i < 5; ++i)
    {
        // result += texture(uAlbedo, lerpedTexCoords + vec2(tex_offset.x * i, 0.0)).rgb *
        // weight[i]; result += texture(uAlbedo, lerpedTexCoords - vec2(tex_offset.x * i, 0.0)).rgb
        // * weight[i];
        result +=
            textureLod(uAlbedo, lerpedTexCoords + vec2(tex_offset.x * i, 0.0), lod) * weight[i];
        result +=
            textureLod(uAlbedo, lerpedTexCoords - vec2(tex_offset.x * i, 0.0), lod) * weight[i];
    }

    fragment = result;
}
#endif