/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

string description = "Basic shaders used by the HKG DX9 shader only (dx9s) renderer";

#ifdef _XBOX
// as we use a proper depth resolve (with bias) we don't need epsilon in here
#define SHADOW_EPSILON 0.0
#else
#define SHADOW_EPSILON 0.0001
#endif

//
// Matrices
//
float4x4 g_mWorld			: World;
float4x4 g_mWorldInv;
float4x4 g_mWorldView		: WorldView;
float4x4 g_mProj			: Proj;
float4x4 g_mWorldViewProj	: WorldViewProjection;
float4x4 g_mWorldInvTranspose			: WorldInverseTranspose;
float4x4 g_mViewInv			: ViewInverse;
float4x4 g_mViewToLightProj; // for shadow maps
 
//
// Textures, set assignment so that the behaviour is the same no mater what shader is using it 
//
sampler g_sSamplerZero : register(s0);    // Shadow Map, or T0
sampler g_sSamplerOne  : register(s1);     // T0 if shadows, otherwise T1
sampler g_sSamplerTwo  : register(s2); // T1 if shadows

//
// Lights 
//
float4 g_vLightDir : Direction  = {0.0f, 1.0f, 0.0f, 1.0f}; // world space
float4 g_cLightColor : Diffuse = {1.0f, 1.0f, 1.0f, 1.0f};

// 
// Material
//
float4 g_cAmbientColor	: MaterialAmbient = {0.0f, 0.0f, 0.0f, 1.0f};
float4 g_cDiffuseColor	: MaterialDiffuse = {1.0f, 1.0f, 1.0f, 1.0f};
float4 g_cSpecularColor	: MaterialSpecular = {1.0f, 1.0f, 1.0f, 1.0f};
float  g_cSpecularPower	: Power = 30.0; // 1..128
float  g_iShadowMapSize		= 1024;

//
// Structures
//
struct vertexInputT2 {
    float3 position			: POSITION;
    float3 normal			: NORMAL;
   #ifdef TEXTURE_ONE
    float2 texCoord0		: TEXCOORD0;
   #endif
   #ifdef TEXTURE_TWO
    float2 texCoord1		: TEXCOORD1;
   #endif
    float4 diffAmbColor		: COLOR0;
};

struct vertexInputPosOnly {
    float3 position			: POSITION;
};

struct vertexInputNoColorT2 {
    float3 position			: POSITION;
    float3 normal			: NORMAL;
    #ifdef TEXTURE_ONE
    float2 texCoord0		: TEXCOORD0;
   #endif
   #ifdef TEXTURE_TWO
    float2 texCoord1		: TEXCOORD1;
   #endif
  
};

struct vertexInputT2B {
	float4 position  : POSITION; 	
	float3 normal    : NORMAL; 	
	 #ifdef TEXTURE_ONE
    float2 texCoord0		: TEXCOORD0;
   #endif
   #ifdef TEXTURE_TWO
    float2 texCoord1		: TEXCOORD1;
   #endif
  float3 tangent   : TEXCOORD2;
	float3 binormal  : TEXCOORD3;	
};

struct vertexOutputT2B {
	float4 position    : POSITION;  // clip space
 #ifdef TEXTURE_ONE
    float2 texCoord0		: TEXCOORD0;
   #endif
   #ifdef TEXTURE_TWO
    float2 texCoord1		: TEXCOORD1;
   #endif
  	float3 L           : TEXCOORD2; // tangent space
	float3 H           : TEXCOORD3;
};

struct vertexOutputT2 {
    float4 position			: POSITION;
  #ifdef TEXTURE_ONE
    float2 texCoord0		: TEXCOORD0;
   #endif
   #ifdef TEXTURE_TWO
    float2 texCoord1		: TEXCOORD1;
   #endif
      float4 diffAmbColor		: COLOR0;
    float4 specCol			: COLOR1;
};

struct vertexOutputShadowDepth {
    float4 position			: POSITION;
    float4 depthZZZW		: TEXCOORD0;
};

struct vertexOutputShadowT2 {
    float4 position			: POSITION;
 #ifdef TEXTURE_ONE
    float2 texCoord0		: TEXCOORD0;
   #endif
   #ifdef TEXTURE_TWO
    float2 texCoord1		: TEXCOORD1;
   #endif
      float4 posLight			: TEXCOORD2;
    float4 diffAmbColor		: COLOR0;
    float4 specCol			: COLOR1;
};

struct vertexOutputShadowT2B {
    float4 position			: POSITION;
 #ifdef TEXTURE_ONE
    float2 texCoord0		: TEXCOORD0;
   #endif
   #ifdef TEXTURE_TWO
    float2 texCoord1		: TEXCOORD1;
   #endif
      float4 posLight			: TEXCOORD2;
    float3 L				: TEXCOORD3;
	float3 H				: TEXCOORD4;
};

struct pixelOutput
{
    float4 color			: COLOR0;  
};


//////////////////////////////////////////////////////////////
// 
// Vertex Shaders
//

// No lighting, just transform and pass on texture (and material as diffuse)
vertexOutputT2 VertNoLight(vertexInputNoColorT2 In) 
{
    vertexOutputT2 Output;
    Output.position = mul( float4(In.position.xyz , 1.0), g_mWorldViewProj);
    Output.diffAmbColor = g_cDiffuseColor;
#ifdef TEXTURE_ONE
    Output.texCoord0 = In.texCoord0;
#endif
#ifdef TEXTURE_TWO
	Output.texCoord1 = In.texCoord1;
#endif
	Output.specCol = float4(0,0,0,0);
    return Output;
}

// No lighting, just transform and pass on texture (and material as diffuse)
vertexOutputShadowT2 VertNoLightShadowProj(vertexInputNoColorT2 In) 
{
    vertexOutputShadowT2 Output;
    float4 viewPos =  mul( float4(In.position.xyz, 1.0), g_mWorldView);
    Output.position = mul( viewPos, g_mProj);
    Output.diffAmbColor = g_cDiffuseColor;
#ifdef TEXTURE_ONE
    Output.texCoord0 = In.texCoord0;
#endif
#ifdef TEXTURE_TWO
	Output.texCoord1 = In.texCoord1;
#endif
    Output.specCol = float4(0,0,0,0);
    
    	// project pos into light space
    Output.posLight = mul( viewPos, g_mViewToLightProj );

    return Output;
}

// No lighting, just transform and pass on texture (and vertex color as diffuse)
vertexOutputT2 VertNoLightVC(vertexInputT2 In) 
{
    vertexOutputT2 Output;
    Output.position = mul( float4(In.position.xyz , 1.0), g_mWorldViewProj);
	Output.diffAmbColor = In.diffAmbColor;
#ifdef TEXTURE_ONE
    Output.texCoord0 = In.texCoord0;
#endif
#ifdef TEXTURE_TWO
	Output.texCoord1 = In.texCoord1;
#endif
	Output.specCol = float4(0,0,0,0);
    return Output;
}

// No lighting, just transform and pass on texture (and vertex color as diffuse)
// but with transformed view pos wrt shadow map too (for prelit shadow recvs in the scene)
vertexOutputShadowT2 VertNoLightVCShadowProj(vertexInputT2 In) 
{
    vertexOutputShadowT2 Output;
    float4 viewPos =  mul( float4(In.position.xyz, 1.0), g_mWorldView);
    
    Output.position = mul( viewPos, g_mProj);
#ifdef TEXTURE_ONE
    Output.texCoord0 = In.texCoord0;
#endif
#ifdef TEXTURE_TWO
	Output.texCoord1 = In.texCoord1;
#endif    
	// project pos into light space
    Output.posLight = mul( viewPos, g_mViewToLightProj );
    
    Output.diffAmbColor = In.diffAmbColor;
    Output.specCol = float4(0,0,0,0);
    return Output;
}

float4 LightVertex( float3 vert, float3 objectNormal, float3 eye )
{
	float4 N = mul( objectNormal, g_mWorldInvTranspose ); //normal vector
    float3 E = normalize( eye - vert ); //eye vector
    float3 L = g_vLightDir.xyz; //light vector
    float3 H = normalize( E + L ); //half angle vector

	//calculate the diffuse and specular contributions
    float  diff = max(0, dot(N,L));
    float  spec = pow( max(0 , dot(N,H) ), g_cSpecularPower);
    if (diff <= 0)
    	spec = 0;
    
	return float4( diff, diff, diff, spec );
}

void MaterialContribution( float4 vertexColor, float4 lightDiffuseAndSpec, out float4 diff, out float4 spec )
{
	float3 ambColor = vertexColor.rgb * g_cDiffuseColor.rgb * g_cAmbientColor.rgb;
    float3 diffColor = vertexColor.rgb * g_cDiffuseColor.rgb * lightDiffuseAndSpec.rgb * g_cLightColor.rgb;
    
    diff.rgb = diffColor.rgb + ambColor.rgb;
    diff.a = vertexColor.a * g_cDiffuseColor.a; 
    
	spec.rgb = g_cSpecularColor.rgb * g_cLightColor.rgb * lightDiffuseAndSpec.w;
    spec.a = 0;
}

// Vertex based lighting (with specular)
vertexOutputT2 VertOneLight(vertexInputNoColorT2 In) 
{
    vertexOutputT2 Output;
    Output.position = mul( float4(In.position.xyz , 1.0), g_mWorldViewProj);
 #ifdef TEXTURE_ONE
    Output.texCoord0 = In.texCoord0;
#endif
#ifdef TEXTURE_TWO
	Output.texCoord1 = In.texCoord1;
#endif

	//calculate our vectors N, E, L, and H
	float3 worldEyePos = g_mViewInv[3].xyz;
    float3 worldVertPos = mul( In.position, g_mWorld ).xyz;
	float4 light = LightVertex(worldVertPos, In.normal, worldEyePos);
	MaterialContribution(float4(1,1,1,1), light, Output.diffAmbColor, Output.specCol);
	
    return Output;
}

// Vertex based lighting (with specular), also projects the position into the shadow map space
vertexOutputShadowT2 VertOneLightShadowProj(vertexInputNoColorT2 In) 
{
    vertexOutputShadowT2 Output;
    
    float4 viewPos =  mul( float4(In.position.xyz, 1.0), g_mWorldView);
    
    Output.position = mul( viewPos, g_mProj);
 #ifdef TEXTURE_ONE
    Output.texCoord0 = In.texCoord0;
#endif
#ifdef TEXTURE_TWO
	Output.texCoord1 = In.texCoord1;
#endif
   
	// project pos into light space
    Output.posLight = mul( viewPos, g_mViewToLightProj );
    
    // Lighting:
    
	//calculate our vectors N, E, L, and H
	float3 worldEyePos = g_mViewInv[3].xyz;
    float3 worldVertPos = mul(In.position, g_mWorld).xyz;
	float4 light = LightVertex(worldVertPos, In.normal, worldEyePos);
	MaterialContribution(float4(1,1,1,1), light, Output.diffAmbColor, Output.specCol);
	
	return Output;
}


// Vertex based lighting (with specular)
vertexOutputT2 VertOneLightVC(vertexInputT2 In) 
{
    vertexOutputT2 Output;
    Output.position = mul( float4(In.position.xyz , 1.0), g_mWorldViewProj);
#ifdef TEXTURE_ONE
    Output.texCoord0 = In.texCoord0;
#endif
#ifdef TEXTURE_TWO
	Output.texCoord1 = In.texCoord1;
#endif

	//calculate our vectors N, E, L, and H
	float3 worldEyePos = g_mViewInv[3].xyz;
    float3 worldVertPos = mul( In.position, g_mWorld ).xyz;
	float4 light = LightVertex(worldVertPos, In.normal, worldEyePos);
	MaterialContribution(In.diffAmbColor, light, Output.diffAmbColor, Output.specCol);
	
    return Output;
}

// Vertex based lighting (with specular), also projects the position into the shadow map space
vertexOutputShadowT2 VertOneLightVCShadowProj(vertexInputT2 In) 
{
    vertexOutputShadowT2 Output;
    
    float4 viewPos =  mul( float4(In.position.xyz, 1.0), g_mWorldView);
    
    Output.position = mul( viewPos, g_mProj);
#ifdef TEXTURE_ONE
    Output.texCoord0 = In.texCoord0;
#endif
#ifdef TEXTURE_TWO
	Output.texCoord1 = In.texCoord1;
#endif    
	// project pos into light space
    Output.posLight = mul( viewPos, g_mViewToLightProj );
    
    // Lighting:
    
	//calculate our vectors N, E, L, and H
	float3 worldEyePos = g_mViewInv[3].xyz;
    float3 worldVertPos = mul(In.position, g_mWorld).xyz;
	float4 light = LightVertex(worldVertPos, In.normal, worldEyePos);
	MaterialContribution(In.diffAmbColor, light, Output.diffAmbColor, Output.specCol);
	
	return Output;
}


vertexOutputT2B VertOneLightBump( vertexInputT2B In )
{
	vertexOutputT2B Output;

	// copy texture coordinates
#ifdef TEXTURE_ONE
    Output.texCoord0 = In.texCoord0;
#endif
#ifdef TEXTURE_TWO
	Output.texCoord1 = In.texCoord1;
#endif
	// transform position to clip space
	Output.position = mul(In.position, g_mWorldViewProj);

	// compute the 3x3 tranform from object space to tangent space
	float3x3 objToTangentSpace;
	
	float bumpHeight = 1.0f;
		
	objToTangentSpace[0] = In.tangent * bumpHeight;
	objToTangentSpace[1] = In.binormal * bumpHeight;
	objToTangentSpace[2] = In.normal;
	
    float4 vertexPos = mul(In.position, g_mWorld); // world space position

	// light vector
	float3 lightVec = mul( g_vLightDir, (float3x3)g_mWorldInv );  // transform back to object space
	Output.L = mul( objToTangentSpace, lightVec ); // transform from object to tangent space
	
	// eye vector
	float3 eyeVec = g_mViewInv[3].xyz - vertexPos.xyz; // world space eye vector
	eyeVec = mul(eyeVec, (float3x3) g_mWorldInv );  // transform back to object space
	eyeVec = normalize(eyeVec);
	
	// half-angle vector
	float3 H = normalize(lightVec + eyeVec);
	Output.H = mul(objToTangentSpace, H);	// transform to tangent space

	return Output;
}


// Vertex based lighting (with specular), also projects the position into the shadow map space
vertexOutputShadowT2B VertOneLightBumpShadowProj( vertexInputT2B In) 
{
    vertexOutputShadowT2B Output;
    
    float4 viewPos =  mul( float4(In.position.xyz, 1.0), g_mWorldView);
    
    Output.position = mul( viewPos, g_mProj);
    
 #ifdef TEXTURE_ONE
    Output.texCoord0 = In.texCoord0;
#endif
#ifdef TEXTURE_TWO
	Output.texCoord1 = In.texCoord1;
#endif
    
		// project pos into light space
    Output.posLight = mul( viewPos, g_mViewToLightProj );
   
	// compute the 3x3 tranform from object space to tangent space
	float3x3 objToTangentSpace;
	
	float bumpHeight = 1.0f;
		
	objToTangentSpace[0] = In.tangent * bumpHeight;
	objToTangentSpace[1] = In.binormal * bumpHeight;
	objToTangentSpace[2] = In.normal;
	
    float4 vertexPos = mul(In.position, g_mWorld); // world space position

	// light vector
	float3 lightVec = mul( g_vLightDir, (float3x3)g_mWorldInv );  // transform back to object space
	Output.L = mul( objToTangentSpace, lightVec ); // transform from object to tangent space
	
	// eye vector
	float3 eyeVec = g_mViewInv[3].xyz - vertexPos.xyz; // world space eye vector
	eyeVec = mul(eyeVec, (float3x3) g_mWorldInv );  // transform back to object space
	eyeVec = normalize(eyeVec);
	
	// half-angle vector
	float3 H = normalize(lightVec + eyeVec);
	Output.H = mul(objToTangentSpace, H);	// transform to tangent space

	
	return Output;
}



// Vertex transform only, with tcoord0 the enough to work out the transformed depth (no bias)
vertexOutputShadowDepth VertShadowDepth(vertexOutputShadowDepth In)
{
    vertexOutputShadowDepth Output;
    Output.position = mul(In.position, g_mWorldViewProj);
    Output.depthZZZW = float4(Output.position.zzz, Output.position.w);
    return Output;
}

//////////////////////////////////////////////////////////////////
//
// Pixel Shaders
//

// No per pixel lighting, just add the input color with the specular, no texture loookup
pixelOutput PixT0( vertexOutputT2 In ) 
{ 
    pixelOutput Output;
    Output.color = In.diffAmbColor + In.specCol;
 
    return Output;
}

#ifdef TEXTURE_ONE
 
// No per pixel lighting, just modulate the input color with the texture and add specular (afterwards)
pixelOutput PixT1( vertexOutputT2 In ) 
{ 
    pixelOutput Output;
    Output.color = tex2D(g_sSamplerZero, In.texCoord0) * In.diffAmbColor;// + In.specCol;
    return Output;
}

#ifdef TEXTURE_TWO

pixelOutput PixT2( vertexOutputT2 In ) 
{ 
    pixelOutput Output;
    Output.color = tex2D(g_sSamplerZero, In.texCoord0) * tex2D(g_sSamplerOne, In.texCoord1) * In.diffAmbColor + In.specCol;
    return Output;
}

#endif
#endif

// Pixel shader to create a color based shadow map
pixelOutput PixShadowMap( vertexOutputShadowDepth In )
{
    pixelOutput Output;
    float depth = (In.depthZZZW.b / In.depthZZZW.a);
    Output.color = float4( depth.xxx, 1.0f );
    return Output;
}


// Blinn/Phong lighting model
float4 Phong(float2 dots) 
{
	float NdotL = dots.x;
	float NdotH = dots.y;
	float diffuse = max(NdotL, 0.0);
	float specular = pow(NdotH, g_cSpecularPower);
	if (NdotL <= 0) specular = 0;
	return float4(diffuse, diffuse, diffuse, specular);
}

#ifdef TEXTURE_TWO
 
// Pixel shaders
float4 PixT2Bump( vertexOutputT2B In ) : COLOR
{
	float4 colorMap = tex2D(g_sSamplerZero, In.texCoord0);
	float3 N = tex2D(g_sSamplerOne, In.texCoord1)*2.0 - 1.0;

	N = normalize(N);

	float NdotL = dot(N, In.L);
	float NdotH = dot(N, In.H);
	float4 light = Phong(float2(NdotL, NdotH));
	
	return g_cAmbientColor*colorMap +light*g_cDiffuseColor*colorMap + light.w*g_cSpecularColor ;
}

#endif
 
///
/// ATI style shadow map lookup
///


float getLightAmountNoHW( float4 posLight )
{
	float2 shadowTexCoord = posLight.xy / posLight.w;
	float  shadowTestDepth = (posLight.z / posLight.w) - SHADOW_EPSILON; //-->make it a bit closer, only use if no way to use _proper_ depth bias etc
				
    // transform to texel space
    float2 texelpos = g_iShadowMapSize * shadowTexCoord;
        
    // Determine the lerp amounts           
    float2 lerps = frac( texelpos );

    //read in bilerp stamp, doing the shadow checks
    float sourcevals[4];
    float oneTexel = 1.0/g_iShadowMapSize;
    
    sourcevals[0] = (tex2D( g_sSamplerZero, shadowTexCoord ) < shadowTestDepth)? 0.0f: 1.0f;  
    sourcevals[1] = (tex2D( g_sSamplerZero, shadowTexCoord + float2(oneTexel, 0) ) < shadowTestDepth)? 0.0f: 1.0f;  
    sourcevals[2] = (tex2D( g_sSamplerZero, shadowTexCoord + float2(0, oneTexel) ) < shadowTestDepth)? 0.0f: 1.0f;  
    sourcevals[3] = (tex2D( g_sSamplerZero, shadowTexCoord + float2(oneTexel, oneTexel) ) < shadowTestDepth)? 0.0f: 1.0f;  
        
    // lerp between the shadow values to calculate our light amount
    float lightAmount = lerp( lerp( sourcevals[0], sourcevals[1], lerps.x ),
                                lerp( sourcevals[2], sourcevals[3], lerps.x ),
                                lerps.y );

	return lightAmount;                                                       
}

// Pixel shader to use a float based shadow map, will modulate with vertex lit color 
// (does not do per pixel lighting, uses vertex color)
pixelOutput PixShadowSceneNoHWT0( vertexOutputShadowT2 In )
{
    pixelOutput Output;

    float lightAmount = getLightAmountNoHW( In.posLight ); 
    float4 diffuse = ( lightAmount * ( 1 - g_cAmbientColor ) + g_cAmbientColor ) * In.diffAmbColor;
    Output.color.rgb = diffuse.rgb + (In.specCol.rgb * lightAmount); // modulate rgb wrt shadow.
    Output.color.a = In.diffAmbColor.a; // modulate alpha as is, shadow doesn't affect it.
    return Output;
}

#ifdef TEXTURE_ONE

// Pixel shader to use a float based shadow map, will modulate with vertex lit color 
// (does not do per pixel lighting, uses vertex color)
pixelOutput PixShadowSceneNoHWT1( vertexOutputShadowT2 In )
{
    pixelOutput Output;

    float lightAmount = getLightAmountNoHW( In.posLight ); 
    float4 diffuse = ( lightAmount * ( 1 - g_cAmbientColor ) + g_cAmbientColor ) * In.diffAmbColor;
    float4 texel = tex2D( g_sSamplerOne, In.texCoord0 );
    Output.color.rgb = texel.rgb * diffuse.rgb + (In.specCol.rgb * lightAmount); // modulate rgb wrt shadow.
    Output.color.a = texel.a * In.diffAmbColor.a; // modulate alpha as is, shadow doesn't affect it.
    return Output;
}

#ifdef TEXTURE_TWO

// Pixel shader to use a float based shadow map, will modulate with vertex lit color 
// (does not do per pixel lighting, uses vertex color)
pixelOutput PixShadowSceneNoHWT2( vertexOutputShadowT2 In )
{
    pixelOutput Output;

    float lightAmount = getLightAmountNoHW( In.posLight ); 
    float4 diffuse = ( lightAmount * ( 1 - g_cAmbientColor ) + g_cAmbientColor ) * In.diffAmbColor;
    float4 texel0 = tex2D( g_sSamplerOne, In.texCoord0 );
    float4 texel1 = tex2D( g_sSamplerTwo, In.texCoord1 );
    Output.color.rgb = texel0.rgb * texel1.rgb * diffuse.rgb + (In.specCol.rgb * lightAmount); // modulate rgb wrt shadow.
    Output.color.a = texel0.a * In.diffAmbColor.a; // modulate alpha as is, shadow doesn't affect it.
    return Output;
}

pixelOutput PixShadowSceneNoHWT2Bump( vertexOutputShadowT2B In )
{
    pixelOutput Output;
    
    float lightAmount = getLightAmountNoHW( In.posLight ); 
    float4 colorMap = tex2D(g_sSamplerOne, In.texCoord0);
	float3 N = tex2D(g_sSamplerTwo, In.texCoord1)*2.0 - 1.0;

	N = normalize(N);

	float NdotL = dot(N, In.L);
	float NdotH = dot(N, In.H);
	float4 light = lightAmount * Phong(float2(NdotL, NdotH));
	
	Output.color.rgb = g_cAmbientColor*colorMap +light*g_cDiffuseColor*colorMap + light.w*g_cSpecularColor ;
	Output.color.a = colorMap.a * g_cDiffuseColor.a; // modulate alpha as is, shadow doesn't affect it.
	
    return Output;
}
#endif
#endif

///
/// NVIDIA style shadow map lookup 
///

// Pixel shader that samples the shadow map assuming that it is a proper hardware depth map 
// and the h/w will do the depth test.
pixelOutput PixShadowSceneHWT0( vertexOutputShadowT2 In )
{ 
	pixelOutput Output;
	float lightAmount = tex2Dproj(g_sSamplerZero, In.posLight).r; // white or black, so red is all we need.
	float4 diffuse = ( lightAmount * ( 1 - g_cAmbientColor ) + g_cAmbientColor ) * In.diffAmbColor;
	Output.color.rgb = diffuse.rgb + (In.specCol.rgb * lightAmount);
	Output.color.a = In.diffAmbColor.a; // modulate alpha as is, shadow doesn't affect it.
	return Output;
}

#ifdef TEXTURE_ONE

// Pixel shader that samples the shadow map assuming that it is a proper hardware depth map 
// and the h/w will do the depth test.
pixelOutput PixShadowSceneHWT1( vertexOutputShadowT2 In )
{ 
	pixelOutput Output;
	float lightAmount = tex2Dproj(g_sSamplerZero, In.posLight).r; // white or black, so red is all we need.
	float4 diffuse = ( lightAmount * ( 1 - g_cAmbientColor ) + g_cAmbientColor ) * In.diffAmbColor;
	float4 texel = tex2D( g_sSamplerOne, In.texCoord0 );
	Output.color.rgb = texel.rgb * diffuse.rgb + (In.specCol.rgb * lightAmount);
	Output.color.a = texel.a * In.diffAmbColor.a; // modulate alpha as is, shadow doesn't affect it.
	return Output;
}

#ifdef TEXTURE_TWO

// Pixel shader that samples the shadow map assuming that it is a proper hardware depth map 
// and the h/w will do the depth test.
pixelOutput PixShadowSceneHWT2( vertexOutputShadowT2 In )
{ 
	pixelOutput Output;
	float lightAmount = tex2Dproj(g_sSamplerZero, In.posLight).r; // white or black, so red is all we need.
	float4 diffuse = ( lightAmount * ( 1 - g_cAmbientColor ) + g_cAmbientColor ) * In.diffAmbColor;
	float4 texel0 = tex2D( g_sSamplerOne, In.texCoord0 );
	float4 texel1 = tex2D( g_sSamplerTwo, In.texCoord1 );
	Output.color.rgb = texel0.rgb * diffuse.rgb + (In.specCol.rgb * lightAmount);
	Output.color.a = texel0.a * In.diffAmbColor.a; // modulate alpha as is, shadow doesn't affect it.
	return Output;
}

// Pixel shader that samples the shadow map assuming that it is a proper hardware depth map 
// and the h/w will do the depth test.
pixelOutput PixShadowSceneHWT2Bump( vertexOutputShadowT2B In )
{ 
	pixelOutput Output;
	float lightAmount = tex2Dproj(g_sSamplerZero, In.posLight).r; // white or black, so red is all we need.
	float4 colorMap = tex2D(g_sSamplerOne, In.texCoord0);
	float3 N = tex2D(g_sSamplerTwo, In.texCoord1)*2.0 - 1.0;

	N = normalize(N);

	float NdotL = dot(N, In.L);
	float NdotH = dot(N, In.H);
	float4 light = lightAmount * Phong( float2(NdotL, NdotH) );
	
	Output.color.rgb = (g_cAmbientColor*colorMap +light*g_cDiffuseColor*colorMap + light.w*g_cSpecularColor).rgb;
	Output.color.a = colorMap.a * g_cDiffuseColor.a; // modulate alpha as is, shadow doesn't affect it.
	return Output;
}

#endif
#endif
/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
