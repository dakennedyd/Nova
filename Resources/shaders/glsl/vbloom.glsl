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

const float offset = 1.0 / 300.0;
void main()
{
    const int lod = 2;

    float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
    vec2 tex_offset = 1.0 / textureSize(uAlbedo, lod) * 2.0; // gets size of single texel
    vec3 result = textureLod(uAlbedo, lerpedTexCoords, lod).rgb *
                  weight[0]; // current fragment's contribution
    // vec3 result = texture(uAlbedo, lerpedTexCoords).rgb * weight[0]; // current fragment's
    // contribution for(int i2 = 0; i2 < 5; ++i2)
    for (int i = 1; i < 5; ++i)
    {
        // result += texture(uAlbedo, lerpedTexCoords + vec2(0.0, tex_offset.y * i)).rgb *
        // weight[i]; result += texture(uAlbedo, lerpedTexCoords - vec2(0.0, tex_offset.y * i)).rgb
        // * weight[i];
        result +=
            textureLod(uAlbedo, lerpedTexCoords + vec2(0.0, tex_offset.y * i), lod).rgb * weight[i];
        result +=
            textureLod(uAlbedo, lerpedTexCoords - vec2(0.0, tex_offset.y * i), lod).rgb * weight[i];
    }

    fragment = vec4(result, 1.0);
}
#endif