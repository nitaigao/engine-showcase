/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//
// One diffuse map along with a normal map, with and without shadows. A common demo shader
// with 'proper' assets 
//
//:STYLE VertOneLightParallax PixT1Parallax LD1 T1 DIFFUSE0 NORMAL0 DISPLACEMENT0 SPECULAR0
//:STYLE VertOneLightParallax PixT1ParallaxShadow LD1 T1 SHADOWMAP DIFFUSE0 NORMAL0 DISPLACEMENT0 SPECULAR0

#include "CommonHeader.hlslh"

// #define PARALLAX_VISUALIZE_LOD 1
// #define PARALLAX_VISUALIZE_MIP_LEVEL 1

// extra matrices
float4x4 g_mWorldInv;
float4x4 g_mWorldView		: WorldView;
float4x4 g_mProj			: Proj;
float4x4 g_mViewInv			: ViewInverse;
 
// extra textures
sampler g_sSamplerOne  : register(s1);		// T0 if shadows, otherwise T1
sampler g_sSamplerTwo  : register(s2);		// T1 if shadows, , otherwise T1
sampler g_sSamplerThree : register(s3);     // T1 if shadows, otherwise T3
sampler g_sSamplerFour  : register(s4);		// T3 if shadows

//float    g_fHeightMapScale = 0.1;         // Describes the useful range of values for the height field

//float2   g_vTextureDims = float2(256,256);            // Specifies texture dimensions for computation of mip level at 
                                    // render time (width, height)

//int      g_nLODThreshold = 1;           // The mip level id for transitioning between the full computation
                                    // for parallax occlusion mapping and the bump mapping computation

//int      g_nMinSamples = 2;             // The minimum number of samples for sampling the height field profile
//int      g_nMaxSamples = 8;             // The maximum number of samples for sampling the height field profile

//float    g_fShadowSoftening = 1;        // Blurring factor for the soft shadows computation


// Vertex Shaders

struct vertexInputT1Parallax
{
	float4 inPositionOS  : POSITION; 
    float2 inTexCoord    : TEXCOORD0;
    float3 vInNormalOS   : NORMAL;
    float3 vInTangentOS  : TEXCOORD3;
    float3 vInBinormalOS : TEXCOORD4;
};
						 
struct vertexOutputT1Parallax
{
    float4 position          : POSITION;
    float2 texCoord          : TEXCOORD0;
    float3 vLightTS          : TEXCOORD1;   // light vector in tangent space, denormalized
    float3 vViewTS           : TEXCOORD2;   // view vector in tangent space, denormalized
    float2 vParallaxOffsetTS : TEXCOORD3;   // Parallax offset vector in tangent space
	float3 vPositionWS       : TEXCOORD4;   // Vertex position in world space
    float3 vNormalWS         : TEXCOORD5;   // Normal vector in world space
    float3 vViewWS           : TEXCOORD6;   // View vector in world space
    float4 vPositionLS       : TEXCOORD7;   // Vertex position in light space
};  


vertexOutputT1Parallax VertOneLightParallax( vertexInputT1Parallax In )
{
	vertexOutputT1Parallax Out;

float    g_fHeightMapScale = 0.03;         // Describes the useful range of values for the height field
	// Transform and output input position 
    float4 viewPos =  mul( float4(In.inPositionOS.xyz, 1.0), g_mWorldView);
    Out.position = mul( viewPos, g_mProj );
       
    // Propagate texture coordinate through:
    Out.texCoord = In.inTexCoord;

    // Transform the normal, tangent and binormal vectors from object space to homogeneous projection space:
    float3 vNormalWS   = mul( In.vInNormalOS,   (float3x3) g_mWorld );
    float3 vTangentWS  = mul( In.vInTangentOS,  (float3x3) g_mWorld );
    float3 vBinormalWS = mul( In.vInBinormalOS, (float3x3) g_mWorld );
    
    // Propagate the world space vertex normal through:   
    Out.vNormalWS = vNormalWS;
    
    vNormalWS   = normalize( vNormalWS );
    vTangentWS  = normalize( vTangentWS );
    vBinormalWS = normalize( vBinormalWS );
    
    // Compute position in world space:
    float4 vPositionWS = mul( In.inPositionOS, g_mWorld );
	Out.vPositionWS = vPositionWS.xyz;
                 
    // Compute and output the world view vector (unnormalized):
    float3 vViewWS = g_mViewInv[3].xyz - vPositionWS;
    Out.vViewWS = vViewWS;

    // Compute denormalized light vector in world space:
    float3 vLightWS = g_vLightDir;
	// project pos into light space
    Out.vPositionLS = mul( viewPos, g_mViewToLightProj );

    // Normalize the light and view vectors and transform it to the tangent space:
    float3x3 mWorldToTangent = float3x3( vTangentWS, vBinormalWS, vNormalWS );
       
    // Propagate the view and the light vectors (in tangent space):
    Out.vLightTS = mul( vLightWS, mWorldToTangent );
    Out.vViewTS  = mul( mWorldToTangent, vViewWS  );
       
    // Compute the ray direction for intersecting the height field profile with 
    // current view ray. See the above paper for derivation of this computation.
         
    // Compute initial parallax displacement direction:
    float2 vParallaxDirection = normalize(  Out.vViewTS.xy );
       
    // The length of this vector determines the furthest amount of displacement:
    float fLength         = length( Out.vViewTS );
    float fParallaxLength = sqrt( fLength * fLength - Out.vViewTS.z * Out.vViewTS.z ) / Out.vViewTS.z; 
       
    // Compute the actual reverse parallax displacement vector:
    Out.vParallaxOffsetTS = vParallaxDirection * fParallaxLength;
       
    // Need to scale the amount of displacement to account for different height ranges
    // in height maps. This is controlled by an artist-editable parameter:
    Out.vParallaxOffsetTS *= g_fHeightMapScale;

	return Out;
}

//--------------------------------------------------------------------------------------
// Function:    ComputeIllumination
// 
// Description: Computes phong illumination for the given pixel using its attribute 
//              textures and a light vector.
//--------------------------------------------------------------------------------------
float4 ComputeIllumination( float2 texCoord, float3 vLightTS, float3 vViewTS)
{
   // Sample the normal from the normal map for the given texture sample:
   float3 vNormalTS = normalize( tex2D( g_sSamplerOne, texCoord ) * 2 - 1 );
   
   // Sample base map:
   float4 cBaseColor = tex2D( g_sSamplerZero, texCoord );
   
   // Compute diffuse color component:
   float3 vLightTSAdj = float3( vLightTS.x, -vLightTS.y, vLightTS.z );
   
   float4 cDiffuse = saturate( dot( vNormalTS, vLightTSAdj )) * g_cDiffuseColor;
   
   // Compute the specular component if desired:  
   float3 vReflectionTS = normalize( 2 * dot( vViewTS, vNormalTS ) * vNormalTS - vViewTS );

   float fRdotL = saturate( dot( vReflectionTS, vLightTSAdj ));
   float4 cSpecular = saturate( pow( fRdotL, g_cSpecularPower )) * 
						tex2D( g_sSamplerThree, texCoord ) * g_cSpecularColor;
   
   // Composite the final color:
   return ( g_cAmbientColor + cDiffuse ) * cBaseColor + cSpecular; 
}   
 
float4 ComputeIlluminationAndShadow( float2 texCoord, 
									 float3 vLightTS, float3 vViewTS, 
									 float4 vPositionLS, float3 vPositionWS, 
									 float fOcclusionShadow )
{
   // Sample the normal from the normal map for the given texture sample:
   float3 vNormalTS = normalize( tex2D( g_sSamplerTwo, texCoord ) * 2 - 1 );
   
   // Sample base map:
   float4 cBaseColor = tex2D( g_sSamplerOne, texCoord );
   
   // Compute diffuse color component:
   float3 vLightTSAdj = float3( vLightTS.x, -vLightTS.y, vLightTS.z );
   
   float4 cDiffuse = saturate( dot( vNormalTS, vLightTSAdj )) * g_cDiffuseColor;
   
   // Compute the specular component if desired:  
   float3 vReflectionTS = normalize( 2 * dot( vViewTS, vNormalTS ) * vNormalTS - vViewTS );

   float fRdotL = saturate( dot( vReflectionTS, vLightTSAdj ));
   float4 cSpecular = saturate( pow( fRdotL, g_cSpecularPower )) * 
						tex2D( g_sSamplerFour, texCoord ) * g_cSpecularColor;
   
#ifdef HKG_SHADOWS_VSM
	float lightAmount = getLightAmountVSM( vPositionLS, vPositionWS );
#else
	float lightAmount = getLightAmountSM( vPositionLS ); 
#endif

   // Composite the final color:
   return (( g_cAmbientColor + cDiffuse ) * cBaseColor + cSpecular ) * lightAmount.xxxx * fOcclusionShadow.xxxx; 
}   
 
// Pixel shaders
pixelOutput PixT1Parallax( vertexOutputT1Parallax In )
{
   //  Normalize the interpolated vectors:
   float3 vViewTS   = normalize( In.vViewTS  );
   float3 vViewWS   = normalize( In.vViewWS  );
   float3 vLightTS  = normalize( In.vLightTS );
   float3 vNormalWS = normalize( In.vNormalWS );

//	pixelOutput o;
//	o.color = ComputeIllumination(In.texCoord, vLightTS, vViewTS, 1);
//	return o;
	float    g_fHeightMapScale = 0.03;         // Describes the useful range of values for the height field
float2   g_vTextureDims = float2(256,256);            // Specifies texture dimensions for computation of mip level at 
                                    // render time (width, height)
int      g_nLODThreshold = 5;           // The mip level id for transitioning between the full computation
                                    // for parallax occlusion mapping and the bump mapping computation

   float4 cResultColor = float4( 0, 0, 0, 1 );
int      g_nMinSamples = 2;             // The minimum number of samples for sampling the height field profile
int      g_nMaxSamples = 20;             // The maximum number of samples for sampling the height field profile

   // Adaptive in-shader level-of-detail system implementation. Compute the 
   // current mip level explicitly in the pixel shader and use this information 
   // to transition between different levels of detail from the full effect to 
   // simple bump mapping. See the above paper for more discussion of the approach
   // and its benefits.
   
   // Compute the current gradients:
   float2 fTexCoordsPerSize = In.texCoord * g_vTextureDims;

   // Compute all 4 derivatives in x and y in a single instruction to optimize:
   float2 dxSize, dySize;
   float2 dx, dy;

   float4( dxSize, dx ) = ddx( float4( fTexCoordsPerSize, In.texCoord ) );
   float4( dySize, dy ) = ddy( float4( fTexCoordsPerSize, In.texCoord ) );
                  
   float  fMipLevel;      
   float  fMipLevelInt;    // mip level integer portion
   float  fMipLevelFrac;   // mip level fractional amount for blending in between levels

   float  fMinTexCoordDelta;
   float2 dTexCoords;

   // Find min of change in u and v across quad: compute du and dv magnitude across quad
   dTexCoords = dxSize * dxSize + dySize * dySize;

   // Standard mipmapping uses max here
   fMinTexCoordDelta = max( dTexCoords.x, dTexCoords.y );

   // Compute the current mip level  (* 0.5 is effectively computing a square root before )
   fMipLevel = max( 0.5 * log2( fMinTexCoordDelta ), 0 );
    
   // Start the current sample located at the input texture coordinate, which would correspond
   // to computing a bump mapping result:
   float2 texSample = In.texCoord;
   
   // Multiplier for visualizing the level of detail (see notes for 'nLODThreshold' variable
   // for how that is done visually)
   float4 cLODColoring = float4( 1, 1, 3, 1 );

   if ( fMipLevel <= (float) g_nLODThreshold )
   {
      //===============================================//
      // Parallax occlusion mapping offset computation //
      //===============================================//

      // Utilize dynamic flow control to change the number of samples per ray 
      // depending on the viewing angle for the surface. Oblique angles require 
      // smaller step sizes to achieve more accurate precision for computing displacement.
      // We express the sampling rate as a linear function of the angle between 
      // the geometric normal and the view direction ray:
      int nNumSteps = (int) lerp( g_nMaxSamples, g_nMinSamples, dot( vViewWS, vNormalWS ) );

      // Intersect the view ray with the height field profile along the direction of
      // the parallax offset ray (computed in the vertex shader. Note that the code is
      // designed specifically to take advantage of the dynamic flow control constructs
      // in HLSL and is very sensitive to specific syntax. When converting to other examples,
      // if still want to use dynamic flow control in the resulting assembly shader,
      // care must be applied.
      // 
      // In the below steps we approximate the height field profile as piecewise linear
      // curve. We find the pair of endpoints between which the intersection between the 
      // height field profile and the view ray is found and then compute line segment
      // intersection for the view ray and the line segment formed by the two endpoints.
      // This intersection is the displacement offset from the original texture coordinate.
      // See the above paper for more details about the process and derivation.
      //

      float fCurrHeight = 0.0;
      float fStepSize   = 1.0 / (float) nNumSteps;
      float fPrevHeight = 1.0;
      float fNextHeight = 0.0;

      int    nStepIndex = 0;
      bool   bCondition = true;

      float2 vTexOffsetPerStep = fStepSize * In.vParallaxOffsetTS;
      float2 vTexCurrentOffset = In.texCoord;
      float  fCurrentBound     = 1.0;
      float  fParallaxAmount   = 0.0;

      float2 pt1 = 0;
      float2 pt2 = 0;
       
      float2 texOffset2 = 0;

      while ( nStepIndex < nNumSteps ) 
      {
         vTexCurrentOffset -= vTexOffsetPerStep;

         // Sample height map which in this case is stored in the alpha channel of the normal map:
         //fCurrHeight = tex2Dgrad( g_sSamplerOne, vTexCurrentOffset, dx, dy ).a;
         fCurrHeight = tex2Dgrad( g_sSamplerTwo, vTexCurrentOffset, dx, dy ).r;

         fCurrentBound -= fStepSize;

         if ( fCurrHeight > fCurrentBound ) 
         {   
            pt1 = float2( fCurrentBound, fCurrHeight );
            pt2 = float2( fCurrentBound + fStepSize, fPrevHeight );

            texOffset2 = vTexCurrentOffset - vTexOffsetPerStep;

            nStepIndex = nNumSteps + 1;
            fPrevHeight = fCurrHeight;
         }
         else
         {
            nStepIndex++;
            fPrevHeight = fCurrHeight;
         }
      }   

      float fDelta2 = pt2.x - pt2.y;
      float fDelta1 = pt1.x - pt1.y;
      
      float fDenominator = fDelta2 - fDelta1;
      
      // SM 3.0 requires a check for divide by zero, since that operation will generate
      // an 'Inf' number instead of 0, as previous models (conveniently) did:
      if ( fDenominator == 0.0 )
      {
         fParallaxAmount = 0.0;
      }
      else
      {
         fParallaxAmount = (pt1.x * fDelta2 - pt2.x * fDelta1 ) / fDenominator;
      }
      
      float2 vParallaxOffset = In.vParallaxOffsetTS * (1 - fParallaxAmount );

      // The computed texture offset for the displaced point on the pseudo-extruded surface:
      float2 texSampleBase = In.texCoord - vParallaxOffset;
      texSample = texSampleBase;

      // Lerp to bump mapping only if we are in between, transition section:
        
      cLODColoring = float4( 1, 1, 1, 1 ); 

      if ( fMipLevel > (float)(g_nLODThreshold - 1) )
      {
         // Lerp based on the fractional part:
         fMipLevelFrac = modf( fMipLevel, fMipLevelInt );

#ifdef PARALLAX_VISUALIZE_LOD
        // For visualizing: lerping from regular POM-resulted color through blue color for transition layer:
        cLODColoring = float4( 1, 1, max( 1, 2 * fMipLevelFrac ), 1 ); 
#endif

         // Lerp the texture coordinate from parallax occlusion mapped coordinate to bump mapping
         // smoothly based on the current mip level:
         texSample = lerp( texSampleBase, In.texCoord, fMipLevelFrac );

	  }  
   } 
   // Compute resulting color for the pixel:
   cResultColor = ComputeIllumination( texSample, vLightTS, vViewTS );
              
#ifdef PARALLAX_VISUALIZE_LOD
   cResultColor *= cLODColoring;
#endif
   
   // Visualize currently computed mip level, tinting the color blue if we are in 
   // the region outside of the threshold level:
#ifdef PARALLAX_VISUALIZE_MIP_LEVEL
   cResultColor = fMipLevel.xxxx;
#endif

   // If using HDR rendering, make sure to tonemap the resuld color prior to outputting it.
   // But since this example isn't doing that, we just output the computed result color here:
   pixelOutput Output;
   Output.color = cResultColor;
   
   return Output;
}

pixelOutput PixT1ParallaxShadow( vertexOutputT1Parallax In )
{
   //  Normalize the interpolated vectors:
   float3 vViewTS   = normalize( In.vViewTS  );
   float3 vViewWS   = normalize( In.vViewWS  );
   float3 vLightTS  = normalize( In.vLightTS );
   float3 vNormalWS = normalize( In.vNormalWS );

//	pixelOutput o;
//	o.color = ComputeIllumination(In.texCoord, vLightTS, vViewTS, 1);
//	return o;
	float    g_fHeightMapScale = 0.03;         // Describes the useful range of values for the height field
float2   g_vTextureDims = float2(256,256);            // Specifies texture dimensions for computation of mip level at 
                                    // render time (width, height)
int      g_nLODThreshold = 5;           // The mip level id for transitioning between the full computation
                                    // for parallax occlusion mapping and the bump mapping computation

   float4 cResultColor = float4( 0, 0, 0, 1 );
int      g_nMinSamples = 2;             // The minimum number of samples for sampling the height field profile
int      g_nMaxSamples = 20;             // The maximum number of samples for sampling the height field profile

float    g_fShadowSoftening = 1;        // Blurring factor for the soft shadows computation

   // Adaptive in-shader level-of-detail system implementation. Compute the 
   // current mip level explicitly in the pixel shader and use this information 
   // to transition between different levels of detail from the full effect to 
   // simple bump mapping. See the above paper for more discussion of the approach
   // and its benefits.
   
   // Compute the current gradients:
   float2 fTexCoordsPerSize = In.texCoord * g_vTextureDims;

   // Compute all 4 derivatives in x and y in a single instruction to optimize:
   float2 dxSize, dySize;
   float2 dx, dy;

   float4( dxSize, dx ) = ddx( float4( fTexCoordsPerSize, In.texCoord ) );
   float4( dySize, dy ) = ddy( float4( fTexCoordsPerSize, In.texCoord ) );
                  
   float  fMipLevel;      
   float  fMipLevelInt;    // mip level integer portion
   float  fMipLevelFrac;   // mip level fractional amount for blending in between levels

   float  fMinTexCoordDelta;
   float2 dTexCoords;

   // Find min of change in u and v across quad: compute du and dv magnitude across quad
   dTexCoords = dxSize * dxSize + dySize * dySize;

   // Standard mipmapping uses max here
   fMinTexCoordDelta = max( dTexCoords.x, dTexCoords.y );

   // Compute the current mip level  (* 0.5 is effectively computing a square root before )
   fMipLevel = max( 0.5 * log2( fMinTexCoordDelta ), 0 );
    
   // Start the current sample located at the input texture coordinate, which would correspond
   // to computing a bump mapping result:
   float2 texSample = In.texCoord;
   
   // Multiplier for visualizing the level of detail (see notes for 'nLODThreshold' variable
   // for how that is done visually)
   float4 cLODColoring = float4( 1, 1, 3, 1 );

   float fOcclusionShadow = 1.0;

   if ( fMipLevel <= (float) g_nLODThreshold )
   {
      //===============================================//
      // Parallax occlusion mapping offset computation //
      //===============================================//

      // Utilize dynamic flow control to change the number of samples per ray 
      // depending on the viewing angle for the surface. Oblique angles require 
      // smaller step sizes to achieve more accurate precision for computing displacement.
      // We express the sampling rate as a linear function of the angle between 
      // the geometric normal and the view direction ray:
      int nNumSteps = (int) lerp( g_nMaxSamples, g_nMinSamples, dot( vViewWS, vNormalWS ) );

      // Intersect the view ray with the height field profile along the direction of
      // the parallax offset ray (computed in the vertex shader. Note that the code is
      // designed specifically to take advantage of the dynamic flow control constructs
      // in HLSL and is very sensitive to specific syntax. When converting to other examples,
      // if still want to use dynamic flow control in the resulting assembly shader,
      // care must be applied.
      // 
      // In the below steps we approximate the height field profile as piecewise linear
      // curve. We find the pair of endpoints between which the intersection between the 
      // height field profile and the view ray is found and then compute line segment
      // intersection for the view ray and the line segment formed by the two endpoints.
      // This intersection is the displacement offset from the original texture coordinate.
      // See the above paper for more details about the process and derivation.
      //

      float fCurrHeight = 0.0;
      float fStepSize   = 1.0 / (float) nNumSteps;
      float fPrevHeight = 1.0;
      float fNextHeight = 0.0;

      int    nStepIndex = 0;
      bool   bCondition = true;

      float2 vTexOffsetPerStep = fStepSize * In.vParallaxOffsetTS;
      float2 vTexCurrentOffset = In.texCoord;
      float  fCurrentBound     = 1.0;
      float  fParallaxAmount   = 0.0;

      float2 pt1 = 0;
      float2 pt2 = 0;
       
      float2 texOffset2 = 0;

      while ( nStepIndex < nNumSteps ) 
      {
         vTexCurrentOffset -= vTexOffsetPerStep;

         // Sample height map which in this case is stored in the alpha channel of the normal map:
         //fCurrHeight = tex2Dgrad( g_sSamplerOne, vTexCurrentOffset, dx, dy ).a;
         fCurrHeight = tex2Dgrad( g_sSamplerTwo, vTexCurrentOffset, dx, dy ).r;

         fCurrentBound -= fStepSize;

         if ( fCurrHeight > fCurrentBound ) 
         {   
            pt1 = float2( fCurrentBound, fCurrHeight );
            pt2 = float2( fCurrentBound + fStepSize, fPrevHeight );

            texOffset2 = vTexCurrentOffset - vTexOffsetPerStep;

            nStepIndex = nNumSteps + 1;
            fPrevHeight = fCurrHeight;
         }
         else
         {
            nStepIndex++;
            fPrevHeight = fCurrHeight;
         }
      }   

      float fDelta2 = pt2.x - pt2.y;
      float fDelta1 = pt1.x - pt1.y;
      
      float fDenominator = fDelta2 - fDelta1;
      
      // SM 3.0 requires a check for divide by zero, since that operation will generate
      // an 'Inf' number instead of 0, as previous models (conveniently) did:
      if ( fDenominator == 0.0 )
      {
         fParallaxAmount = 0.0;
      }
      else
      {
         fParallaxAmount = (pt1.x * fDelta2 - pt2.x * fDelta1 ) / fDenominator;
      }
      
      float2 vParallaxOffset = In.vParallaxOffsetTS * (1 - fParallaxAmount );

      // The computed texture offset for the displaced point on the pseudo-extruded surface:
      float2 texSampleBase = In.texCoord - vParallaxOffset;
      texSample = texSampleBase;

      // Lerp to bump mapping only if we are in between, transition section:
        
      cLODColoring = float4( 1, 1, 1, 1 ); 

      if ( fMipLevel > (float)(g_nLODThreshold - 1) )
      {
         // Lerp based on the fractional part:
         fMipLevelFrac = modf( fMipLevel, fMipLevelInt );

#ifdef PARALLAX_VISUALIZE_LOD
        // For visualizing: lerping from regular POM-resulted color through blue color for transition layer:
        cLODColoring = float4( 1, 1, max( 1, 2 * fMipLevelFrac ), 1 ); 
#endif

         // Lerp the texture coordinate from parallax occlusion mapped coordinate to bump mapping
         // smoothly based on the current mip level:
         texSample = lerp( texSampleBase, In.texCoord, fMipLevelFrac );

     }  

	// calculate self-shadowing
     {
        float2 vLightRayTS = vLightTS.xy * g_fHeightMapScale;
      
        // Compute the soft blurry shadows taking into account self-occlusion for 
        // features of the height field:
   
        float sh0 =  tex2Dgrad( g_sSamplerTwo, texSampleBase, dx, dy ).r;
        float shA = (tex2Dgrad( g_sSamplerTwo, texSampleBase + vLightRayTS * 0.88, dx, dy ).r - sh0 - 0.88 ) *  1 * g_fShadowSoftening;
        float sh9 = (tex2Dgrad( g_sSamplerTwo, texSampleBase + vLightRayTS * 0.77, dx, dy ).r - sh0 - 0.77 ) *  2 * g_fShadowSoftening;
        float sh8 = (tex2Dgrad( g_sSamplerTwo, texSampleBase + vLightRayTS * 0.66, dx, dy ).r - sh0 - 0.66 ) *  4 * g_fShadowSoftening;
        float sh7 = (tex2Dgrad( g_sSamplerTwo, texSampleBase + vLightRayTS * 0.55, dx, dy ).r - sh0 - 0.55 ) *  6 * g_fShadowSoftening;
        float sh6 = (tex2Dgrad( g_sSamplerTwo, texSampleBase + vLightRayTS * 0.44, dx, dy ).r - sh0 - 0.44 ) *  8 * g_fShadowSoftening;
        float sh5 = (tex2Dgrad( g_sSamplerTwo, texSampleBase + vLightRayTS * 0.33, dx, dy ).r - sh0 - 0.33 ) * 10 * g_fShadowSoftening;
        float sh4 = (tex2Dgrad( g_sSamplerTwo, texSampleBase + vLightRayTS * 0.22, dx, dy ).r - sh0 - 0.22 ) * 12 * g_fShadowSoftening;
   
        // Compute the actual shadow strength:
        fOcclusionShadow = 1 - max( max( max( max( max( max( shA, sh9 ), sh8 ), sh7 ), sh6 ), sh5 ), sh4 );
      
        // The previous computation overbrightens the image, let's adjust for that:
        fOcclusionShadow = fOcclusionShadow * 0.6 + 0.4;         
     }       
   }   

   // Compute resulting color for the pixel:
   cResultColor = ComputeIlluminationAndShadow( texSample, vLightTS, vViewTS, In.vPositionLS, In.vPositionWS, fOcclusionShadow );
              
#ifdef PARALLAX_VISUALIZE_LOD
   cResultColor *= cLODColoring;
#endif
   
   // Visualize currently computed mip level, tinting the color blue if we are in 
   // the region outside of the threshold level:
#ifdef PARALLAX_VISUALIZE_MIP_LEVEL
   cResultColor = fMipLevel.xxxx;
#endif

   // If using HDR rendering, make sure to tonemap the resuld color prior to outputting it.
   // But since this example isn't doing that, we just output the computed result color here:
   pixelOutput Output;
   Output.color = cResultColor;
   
   return Output;
}

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
