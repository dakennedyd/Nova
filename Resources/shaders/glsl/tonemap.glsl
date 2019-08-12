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
uniform float uTime;
out vec4 fragment;

//shader Created by hornet in 2014-11-08
//https://www.youtube.com/watch?v=XisQsjDkuvU

// this software is released into public domain
// For more information, please refer to http://unlicense.org/

float sat( float t ) {
	return clamp( t, 0.0, 1.0 );
}
vec2 sat( vec2 t ) {
	return clamp( t, 0.0, 1.0 );
}

//note: [0;1]
float rand( vec2 n ) {
  return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

//note: [-1;1]
float srand( vec2 n ) {
	return rand(n) * 2.0 - 1.0;
}

float mytrunc( float x, float num_levels )
{
	return floor(x*num_levels) / num_levels;
}
vec2 mytrunc( vec2 x, vec2 num_levels )
{
	return floor(x*num_levels) / num_levels;
}

vec3 rgb2yuv( vec3 rgb )
{
	vec3 yuv;
	yuv.x = dot( rgb, vec3(0.299,0.587,0.114) );
	yuv.y = dot( rgb, vec3(-0.14713, -0.28886, 0.436) );
	yuv.z = dot( rgb, vec3(0.615, -0.51499, -0.10001) );
	return yuv;
 }
 vec3 yuv2rgb( vec3 yuv )
 {
	vec3 rgb;
	rgb.r = yuv.x + yuv.z * 1.13983;
	rgb.g = yuv.x + dot( vec2(-0.39465, -0.58060), yuv.yz );
	rgb.b = yuv.x + yuv.y * 2.03211;
	return rgb;
 }

 vec4 glitch(float amount)
 {
	float THRESHOLD = amount;
	float time_s = mod( uTime, 32.0 );

	float glitch_threshold = 1.0 - THRESHOLD;
	const float max_ofs_siz = 0.1; //TOOD: input
	const float yuv_threshold = 0.5; //TODO: input, >1.0f == no distort
	const float time_frq = 16.0;

	vec2 uv = lerpedTexCoords;
    //uv.y = 1.0 -  uv.y;
	
	const float min_change_frq = 4.0;
	float ct = mytrunc( time_s, min_change_frq );
	float change_rnd = rand( mytrunc(uv.yy,vec2(16)) + 150.0 * ct );

	float tf = time_frq*change_rnd;

	float t = 5.0 * mytrunc( time_s, tf );
	float vt_rnd = 0.5*rand( mytrunc(uv.yy + t, vec2(11)) );
	vt_rnd += 0.5 * rand(mytrunc(uv.yy + t, vec2(7)));
	vt_rnd = vt_rnd*2.0 - 1.0;
	vt_rnd = sign(vt_rnd) * sat( ( abs(vt_rnd) - glitch_threshold) / (1.0-glitch_threshold) );

	vec2 uv_nm = uv;
	uv_nm = sat( uv_nm + vec2(max_ofs_siz*vt_rnd, 0) );

	float rnd = rand( vec2( mytrunc( time_s, 8.0 )) );
	uv_nm.y = (rnd>mix(1.0, 0.975, sat(THRESHOLD))) ? 1.0-uv_nm.y : uv_nm.y;

	vec4 smpl = texture( uRenderedImage, uv_nm, -10.0 );
	vec3 smpl_yuv = rgb2yuv( smpl.rgb );
	smpl_yuv.y /= 1.0-3.0*abs(vt_rnd) * sat( yuv_threshold - vt_rnd );
	smpl_yuv.z += 0.125 * vt_rnd * sat( vt_rnd - yuv_threshold );

	return vec4( yuv2rgb(smpl_yuv), smpl.a );
 }

 //=======================================================================
vec4 chromatic(int sampleCount, float blur, float falloff)
{	
    vec2 direction = normalize(lerpedTexCoords - 0.5); 
	//vec2 direction = normalize(vec2(lerpedTexCoords.x,0.0) -vec2(0.5,0.0)); 
	vec2 velocity = direction * blur * pow(length(lerpedTexCoords - 0.5), falloff);
    //vec2 velocity = direction * blur * pow(length(vec2(lerpedTexCoords.x,0.0) -vec2(0.5,0.0)), falloff);
	float inverseSampleCount = 1.0 / float(sampleCount); 
    
    mat3x2 increments = mat3x2(velocity * 1.0 * inverseSampleCount,
                               velocity * 2.0 * inverseSampleCount,
                               velocity * 4.0 * inverseSampleCount);

    vec3 accumulator = vec3(0);
    mat3x2 offsets = mat3x2(0); 
    
    for (int i = 0; i < sampleCount; i++) {
        accumulator.r += texture(uRenderedImage, lerpedTexCoords + offsets[0]).r; 
        accumulator.g += texture(uRenderedImage, lerpedTexCoords + offsets[1]).g; 
        accumulator.b += texture(uRenderedImage, lerpedTexCoords + offsets[2]).b; 
        
        offsets -= increments;
    }

	return vec4(accumulator / float(sampleCount), 1.0);
}
//=========================================================================

//from https://www.shadertoy.com/view/ls3GWS
#define FXAA_SPAN_MAX 4.0
#define FXAA_REDUCE_MUL   (1.0/FXAA_SPAN_MAX)
#define FXAA_REDUCE_MIN   (1.0/128.0)
#define FXAA_SUBPIX_SHIFT (1.0/4.0)

vec3 FxaaPixelShader( vec4 uv, sampler2D tex, vec2 rcpFrame) {
    
    vec3 rgbNW = textureLod(tex, uv.zw, 0.0).xyz;
    vec3 rgbNE = textureLod(tex, uv.zw + vec2(1,0)*rcpFrame.xy, 0.0).xyz;
    vec3 rgbSW = textureLod(tex, uv.zw + vec2(0,1)*rcpFrame.xy, 0.0).xyz;
    vec3 rgbSE = textureLod(tex, uv.zw + vec2(1,1)*rcpFrame.xy, 0.0).xyz;
    vec3 rgbM  = textureLod(tex, uv.xy, 0.0).xyz;

    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
        FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
    
    dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
          max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
          dir * rcpDirMin)) * rcpFrame.xy;

    vec3 rgbA = (1.0/2.0) * (
        textureLod(tex, uv.xy + dir * (1.0/3.0 - 0.5), 0.0).xyz +
        textureLod(tex, uv.xy + dir * (2.0/3.0 - 0.5), 0.0).xyz);
    vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
        textureLod(tex, uv.xy + dir * (0.0/3.0 - 0.5), 0.0).xyz +
        textureLod(tex, uv.xy + dir * (3.0/3.0 - 0.5), 0.0).xyz);
    
    float lumaB = dot(rgbB, luma);

    if((lumaB < lumaMin) || (lumaB > lumaMax)) return rgbA;
    
    return rgbB; 
}

void main()
{
	vec2 rcpFrame = 1.0/vec2(1440.0,900.0);
	vec4 uv = vec4( lerpedTexCoords, lerpedTexCoords - (rcpFrame * (0.5 + FXAA_SUBPIX_SHIFT)));
	//fragment = vec4(FxaaPixelShader(uv, uRenderedImage, rcpFrame),1.0);// + texture(uBlurredImage, lerpedTexCoords)*0.59;
    //fragment = texture(uRenderedImage, lerpedTexCoords)*0.65 + texture(uBlurredImage, lerpedTexCoords)*0.09;
	//fragment = texture(uBlurredImage, lerpedTexCoords);
  	//fragment = (texture(uRenderedImage, lerpedTexCoords) + texture(uBlurredImage, lerpedTexCoords))+glitch(0.4);
	//fragment = (texture(uRenderedImage, lerpedTexCoords) + texture(uBlurredImage, lerpedTexCoords))+glitch(0.4)+chromatic(50, 0.25,2.0+sin(uTime));
	fragment = chromatic(50, 0.25,2.0+sin(uTime));
	//fragment = glitch(0.8);
	//fragment = glitch(0.2) + chromatic(50, 0.25,2.0+sin(uTime));
    // gamma correct
    //fragment = pow(fragment, vec4(2.2));    
    // HDR tonemapping
    fragment = fragment / (fragment + vec4(1.0));    
    fragment = vec4(fragment.xyz, 1.0);
}
#endif
