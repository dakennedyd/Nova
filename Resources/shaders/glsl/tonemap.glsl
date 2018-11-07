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
uniform sampler2D uBlurredImage;
uniform sampler2D uRenderedImage;
out vec4 fragment;

// const float offset = 1.0 / 300.0;
void main()
{

    /* vec2 offsets[9] = vec2[](
         vec2(-offset,  offset), // top-left
         vec2( 0.0f,    offset), // top-center
         vec2( offset,  offset), // top-right
         vec2(-offset,  0.0f),   // center-left
         vec2( 0.0f,    0.0f),   // center-center
         vec2( offset,  0.0f),   // center-right
         vec2(-offset, -offset), // bottom-left
         vec2( 0.0f,   -offset), // bottom-center
         vec2( offset, -offset)  // bottom-right
     );

     float kernel[9] = float[]( //sharpen
         -1, -1, -1,
         -1,  9, -1,
         -1, -1, -1
     );*/

    // float kernel[9] = float[]( //blur kinda
    // 1.0 / 16, 2.0 / 16, 1.0 / 16,
    // 2.0 / 16, 4.0 / 16, 2.0 / 16,
    // 1.0 / 16, 2.0 / 16, 1.0 / 16
    //);

    // float kernel[9] = float[]( //edge detection
    //    1,  1,  1,
    //    1, -8,  1,
    //    1,  1,  1
    //);

    /*vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(albedo, lerpedTexCoords + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    */

    // fragment =  texture(uRenderedImage, lerpedTexCoords);
    fragment = // textureLod( uBlurredImage, lerpedTexCoords, 0.0)// * 0.001
        textureLod(uBlurredImage, lerpedTexCoords, 3.0) * 0.2
        // + textureLod( uBlurredImage, lerpedTexCoords, 3.0) * 0.01
        + texture(uRenderedImage, lerpedTexCoords);
    // fragment =  texture( uBlurredImage, lerpedTexCoords);
    // fragment = textureLod(albedo, lerpedTexCoords,3);
    // fragment =  texture( albedo, lerpedTexCoords) * textureLod(albedo, lerpedTexCoords+1.0 /
    // textureSize(albedo, 4),4); fragment = texture( albedo, lerpedTexCoords + 0.005*vec2(
    // sin(102.0*lerpedTexCoords.x),cos(76.0*lerpedTexCoords.y)) ); fragment = vec4(vec3(1.0 -
    // texture(albedo, lerpedTexCoords)), 1.0);
    // float average = 0.2126 * fragment.r + 0.7152 * fragment.g + 0.0722 * fragment.b;
    // fragment = vec4(average, average, average, 1.0);

    // HDR tonemapping
    fragment = fragment / (fragment + vec4(1.0));
    // gamma correct
    fragment = pow(fragment, vec4(1.0 / 2.2));
}
#endif