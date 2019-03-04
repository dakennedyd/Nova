#version 330 core
#ifdef NOVA_VERTEX_SHADER

layout (location = 0) in vec3 vertPos;
layout(location = 1) in vec2 texCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uNormalMat;

out vec3 lerpedTexCoords;
out vec2 uv;
out vec3 fragPos;
out vec3 normal;

void main () 
{	
    uv = texCoords;
	lerpedTexCoords = vertPos;
	fragPos = vec3(uModel * vec4(vertPos, 1.0));	
	//mat4 NormalMat = mat4(transpose(inverse(mat3(uModel))));
    normal = normalize(mat3(uModel)*vertPos);  
	gl_Position = uProj * uView * uModel * vec4 (vertPos, 1.0);
}
#endif
#ifdef NOVA_FRAGMENT_SHADER
// implementation of the blumblumshub hash
// as described in MNoise paper http://www.cs.umbc.edu/~olano/papers/mNoise.pdf
vec4 BBS_coord_prepare(vec4 x) {
    // return mod( x, 61.0 ); // unoptimized reference
    return x - floor(x * ( 1.0 / 61.0 )) * 61.0;
}
vec4 BBS_permute(vec4 x) {
    // return mod( x * x, 61.0 ); // unoptimized reference
    return fract( sin(x * x * ( 1.0 / 61.0 ))) * 61.0;
}
vec4 BBS_permute_and_resolve(vec4 x) {
    // return mod( x * x, 61.0 ) / 61.0; // unoptimized reference
    return fract( sin(x * x * ( 1.0 / 61.0 ) ));
}
vec4 BBS_hash_hq( vec3 gridcell ) {
        gridcell += vec3(1);//vec3(3.15,114.77,64.38);
    //    gridcell is assumed to be an integer coordinate
    vec4 hash_coord = BBS_coord_prepare( vec4( gridcell, gridcell + .0 ) );
    vec4 hash = BBS_permute( hash_coord.xzxz /* * 7.0 */ ); // * 7.0 will increase variance close to origin
    hash = BBS_permute( hash + hash_coord.yyww );
    return BBS_permute_and_resolve( hash + hash_coord.xzxz );
}

// The MIT License
// Copyright © 2017 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
// to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
uniform float uTime;

vec3 hash3( vec3 p ){
	p = vec3( dot(p,vec3(127.1,311.7, 74.7)),
			  dot(p,vec3(269.5,183.3,246.1)),
			  dot(p,vec3(113.5,271.9,124.6)));

	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
    //return fract(sin(p)*43758.5453123);
}
float hash1( vec3 p ){	
	return fract(sin(dot(p ,vec3(12.9898,78.233, 312.67))) * 43758.5453);
}

// Computes the analytic derivatives of a 3D Gradient Noise. This can be used for example to compute normals to a
// 3d rocks based on Gradient Noise without approximating the gradient by having to take central differences. More
// info here: http://iquilezles.org/www/articles/gradientnoise/gradientnoise.htm
// return value noise (in x) and its derivatives (in yzw)
vec4 noised( in vec3 x ){
    // grid
    vec3 p = floor(x);
    vec3 w = fract(x);
    
    #if 1
    // quintic interpolant
    vec3 u = w*w*w*(w*(w*6.0-15.0)+10.0);
    vec3 du = 30.0*w*w*(w*(w-2.0)+1.0);
    #else
    // cubic interpolant
    vec3 u = w*w*(3.0-2.0*w);
    vec3 du = 6.0*w*(1.0-w);
    #endif    
    
    // gradients
    vec3 ga = hash3( p+vec3(0.0,0.0,0.0) );
    vec3 gb = hash3( p+vec3(1.0,0.0,0.0) );
    vec3 gc = hash3( p+vec3(0.0,1.0,0.0) );
    vec3 gd = hash3( p+vec3(1.0,1.0,0.0) );
    vec3 ge = hash3( p+vec3(0.0,0.0,1.0) );
	vec3 gf = hash3( p+vec3(1.0,0.0,1.0) );
    vec3 gg = hash3( p+vec3(0.0,1.0,1.0) );
    vec3 gh = hash3( p+vec3(1.0,1.0,1.0) );
    
    // projections
    float va = dot( ga, w-vec3(0.0,0.0,0.0) );
    float vb = dot( gb, w-vec3(1.0,0.0,0.0) );
    float vc = dot( gc, w-vec3(0.0,1.0,0.0) );
    float vd = dot( gd, w-vec3(1.0,1.0,0.0) );
    float ve = dot( ge, w-vec3(0.0,0.0,1.0) );
    float vf = dot( gf, w-vec3(1.0,0.0,1.0) );
    float vg = dot( gg, w-vec3(0.0,1.0,1.0) );
    float vh = dot( gh, w-vec3(1.0,1.0,1.0) );
	
    // interpolations
    return vec4( va + u.x*(vb-va) + u.y*(vc-va) + u.z*(ve-va) + u.x*u.y*(va-vb-vc+vd) + u.y*u.z*(va-vc-ve+vg) + u.z*u.x*(va-vb-ve+vf) + (-va+vb+vc-vd+ve-vf-vg+vh)*u.x*u.y*u.z,    // value
                 ga + u.x*(gb-ga) + u.y*(gc-ga) + u.z*(ge-ga) + u.x*u.y*(ga-gb-gc+gd) + u.y*u.z*(ga-gc-ge+gg) + u.z*u.x*(ga-gb-ge+gf) + (-ga+gb+gc-gd+ge-gf-gg+gh)*u.x*u.y*u.z +   // derivatives
                 du * (vec3(vb,vc,ve) - va + u.yzx*vec3(va-vb-vc+vd,va-vc-ve+vg,va-vb-ve+vf) + u.zxy*vec3(va-vb-ve+vf,va-vb-vc+vd,va-vc-ve+vg) + u.yzx*u.zxy*(-va+vb+vc-vd+ve-vf-vg+vh) ));
}
vec4 noised_BBS_hash_hq( in vec3 x ){
    // grid
    vec3 p = floor(x);
    vec3 w = fract(x);
    
    #if 1
    // quintic interpolant
    vec3 u = w*w*w*(w*(w*6.0-15.0)+10.0);
    vec3 du = 30.0*w*w*(w*(w-2.0)+1.0);
    #else
    // cubic interpolant
    vec3 u = w*w*(3.0-2.0*w);
    vec3 du = 6.0*w*(1.0-w);
    #endif    
    
    // gradients
    vec3 ga = BBS_hash_hq( p+vec3(0.0,0.0,0.0) ).xyz;
    vec3 gb = BBS_hash_hq( p+vec3(1.0,0.0,0.0) ).xyz;
    vec3 gc = BBS_hash_hq( p+vec3(0.0,1.0,0.0) ).xyz;
    vec3 gd = BBS_hash_hq( p+vec3(1.0,1.0,0.0) ).xyz;
    vec3 ge = BBS_hash_hq( p+vec3(0.0,0.0,1.0) ).xyz;
	vec3 gf = BBS_hash_hq( p+vec3(1.0,0.0,1.0) ).xyz;
    vec3 gg = BBS_hash_hq( p+vec3(0.0,1.0,1.0) ).xyz;
    vec3 gh = BBS_hash_hq( p+vec3(1.0,1.0,1.0) ).xyz;
    
    // projections
    float va = dot( ga, w-vec3(0.0,0.0,0.0) );
    float vb = dot( gb, w-vec3(1.0,0.0,0.0) );
    float vc = dot( gc, w-vec3(0.0,1.0,0.0) );
    float vd = dot( gd, w-vec3(1.0,1.0,0.0) );
    float ve = dot( ge, w-vec3(0.0,0.0,1.0) );
    float vf = dot( gf, w-vec3(1.0,0.0,1.0) );
    float vg = dot( gg, w-vec3(0.0,1.0,1.0) );
    float vh = dot( gh, w-vec3(1.0,1.0,1.0) );
	
    // interpolations
    return vec4( va + u.x*(vb-va) + u.y*(vc-va) + u.z*(ve-va) + u.x*u.y*(va-vb-vc+vd) + u.y*u.z*(va-vc-ve+vg) + u.z*u.x*(va-vb-ve+vf) + (-va+vb+vc-vd+ve-vf-vg+vh)*u.x*u.y*u.z,    // value
                 ga + u.x*(gb-ga) + u.y*(gc-ga) + u.z*(ge-ga) + u.x*u.y*(ga-gb-gc+gd) + u.y*u.z*(ga-gc-ge+gg) + u.z*u.x*(ga-gb-ge+gf) + (-ga+gb+gc-gd+ge-gf-gg+gh)*u.x*u.y*u.z +   // derivatives
                 du * (vec3(vb,vc,ve) - va + u.yzx*vec3(va-vb-vc+vd,va-vc-ve+vg,va-vb-ve+vf) + u.zxy*vec3(va-vb-ve+vf,va-vb-vc+vd,va-vc-ve+vg) + u.yzx*u.zxy*(-va+vb+vc-vd+ve-vf-vg+vh) ));
}
float noise( in vec3 x ){
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);
	
    return mix(mix(mix( hash1(p+vec3(0,0,0)), 
                        hash1(p+vec3(1,0,0)),f.x),
                   mix( hash1(p+vec3(0,1,0)), 
                        hash1(p+vec3(1,1,0)),f.x),f.y),
               mix(mix( hash1(p+vec3(0,0,1)), 
                        hash1(p+vec3(1,0,1)),f.x),
                   mix( hash1(p+vec3(0,1,1)), 
                        hash1(p+vec3(1,1,1)),f.x),f.y),f.z);
}

//from http://iquilezles.org/www/articles/morenoise/morenoise.htm
const mat3 m3  = mat3( 0.00,  0.80,  0.60,
                      -0.80,  0.36, -0.48,
                      -0.60, -0.48,  0.64 );
const mat3 m3i = mat3( 0.00, -0.80, -0.60,
                       0.80,  0.36, -0.48,
                       0.60, -0.48,  0.64 );
// returns 3D fbm and its 3 derivatives
//x=noise value   yzw = derivatives
vec4 fbmd( in vec3 x, int octaves) {
    float f = 1.98;  // could be 2.0
    float s = 0.49;  // could be 0.5
    float a = 0.0;
    float b = 0.5;
    vec3  d = vec3(0.0);
    mat3  m = mat3(1.0,0.0,0.0,
                   0.0,1.0,0.0,
                   0.0,0.0,1.0);
    for( int i=0; i < octaves; i++ )
    {
        vec4 n = noised(x);
        a += b*n.x;          // accumulate values		
        d += b*m*n.yzw;      // accumulate derivatives
        b *= s;
        x = f*m3*x;
        m = f*m3i*m;
    }
    return normalize(vec4( a, d ));
}
vec4 fbmd_BBS( in vec3 x, int octaves) {
    float f = 1.98;  // could be 2.0
    float s = 0.49;  // could be 0.5
    float a = 0.0;
    float b = 0.5;
    vec3  d = vec3(0.0);
    mat3  m = mat3(1.0,0.0,0.0,
                   0.0,1.0,0.0,
                   0.0,0.0,1.0);
    for( int i=0; i < octaves; i++ )
    {
        vec4 n = noised_BBS_hash_hq(x);
        a += b*n.x;          // accumulate values		
        d += b*m*n.yzw;      // accumulate derivatives
        b *= s;
        x = f*m3*x;
        m = f*m3i*m;
    }
    return normalize(vec4( a, d ));
}

float fractalNoiseLow(vec3 m) {
    return  0.25*noise(m)
			+0.25*noise(2.0*m);
			//+0.1333333*noise(4.0*m);
			// +0.1666667*noise(8.0*m)
			// //+0.0666667*noise(16.0*m)
			// //+0.0666667*noise(32.0*m)
			// +0.0666667*noise(64.0*m)
			// +0.0666667*noise(128.0*m);
}
float fractalNoiseMed(vec3 m) {
    return  0.5333333*noise(m)
			//+0.82666667*noise(2.0*m)
			+0.1333333*noise(4.0*m)
			+0.1333333*noise(8.0*m)
			//+0.0666667*noise(16.0*m)
			//+0.0666667*noise(32.0*m)
			+0.0266667*noise(64.0*m)
			+0.0266667*noise(128.0*m);
}
float fractalNoiseHigh(vec3 m) {
    return  // 0.5333333*noise(m)
			//+1.82666667*noise(2.0*m)
			//+0.1333333*noise(4.0*m)
			+2.0666667*noise(8.0*m)
			+1.0666667*noise(16.0*m)
			+1.0666667*noise(32.0*m)
			//+1.0666667*noise(64.0*m)
			//+1.0666667*noise(128.0*m);
			+1.0666667*noise(256.0*m)
			+1.0666667*noise(512.0*m);
}

//Fragment Shader with Cube Texture
in vec3 lerpedTexCoords;
in vec2 uv;
in vec3 fragPos;
in vec3 normal;

uniform mat4 uView;
uniform mat4 uModel;
uniform sampler2D uAlbedoMap;
uniform sampler2D uDetailsMap;// RED CHANNEL = metalness GREEN CHANNEL = roughness BLUE CHANNEL =
                              // ambient occlusion
uniform sampler2D uNormalMap;

layout(location = 0) out vec4 gPosMetal;
layout(location = 1) out vec4 gNormRough;
layout(location = 2) out vec4 gAlbedoSkyboxmask;
layout(location = 3) out vec4 gNormalMapAO;

void main () 
{		
	vec3 ocean = vec3(13.0 / 255.0, 55.0 / 255.0, 79.0 / 255.0);
	vec3 ice = vec3(250.0 / 255.0, 250.0 / 255.0, 250.0 / 255.0);	
	
    gPosMetal = vec4(fragPos, 0.0);    
    gNormRough.xyz = normalize(normal);
	gNormalMapAO = vec4(0.4939,0.4939,1.0,1.5);	           

    float n = abs(dot(normal,vec3(0,1,0)));
    //float n = (lerpedTexCoords.y * normal.y);
    n=5.5*n*n*n*n*n;
	vec3 surface;
    //float n = fbmd(lerpedTexCoords,7).x;
    //vec4 f = (1 + sin( (lerpedTexCoords.x + fbmd(vec3(lerpedTexCoords.x * 5 , lerpedTexCoords.y * 5,lerpedTexCoords.z * 5),6 ) / 3. ) * 100.) ) / 3.;
    vec4 f = (fbmd_BBS(lerpedTexCoords,8));
        //float u = 0.5 - 0.5 * atan(lerpedTexCoords.x, -lerpedTexCoords.z) * 1.0/3.14159;
        //float v = 1.0 - acos(lerpedTexCoords.y) * 1.0/3.14159;
    if(f.x > 0)//land
    {
        //float u = 0.5 - 0.5 * atan(lerpedTexCoords.x, -lerpedTexCoords.z) * 1.0/3.14159;
        //float v = 1.0 - acos(lerpedTexCoords.y) * 1.0/3.14159;
        surface = (vec3(.5) + (f.x)) * 
            mix(vec3(.27,.45,.01),vec3(.14,.16,.05),clamp(noised(lerpedTexCoords*4).yzw,0,1));
        gNormRough.w = .8;

    }else{//water
        gNormRough.w = .3;
        //surface = ocean;// * ( ((f.yzw)+2)) * .5;
        surface = ocean*.5;// * ( abs(vec3(fbmd(lerpedTexCoords,2).x)+1)) * .5;
    }
    vec4 clouds = clamp(fbmd((lerpedTexCoords*.8+(uTime*.005))*5,3),0,1);
    surface = mix(mix(vec3(abs(fbmd(lerpedTexCoords+16,8).x*50)),surface,.95),clouds.xxx,.5);
    
    // gNormRough.xyz = texture(uNormalMap,vec2(u,v)).xyz;
    //surface = mix(vec3(n*abs(f.x)),surface,.5);
	gAlbedoSkyboxmask =vec4(surface,0.0);    
}
#endif