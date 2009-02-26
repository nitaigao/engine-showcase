/*********************************************************************NVMH3****
File:  $Id: Scene_bloom.fx

Copyright NVIDIA Corporation 2004
TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
*AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Comments:
	Glow/bloom post processing effect
	Down-samples scene first for performance (reduces size by 4)
	Thresholds luminance for extra highlights
	Separable filter, filters in X, then in Y
	Takes advantage of bilinear filtering for blur

******************************************************************************/

float4 ClearColor : DIFFUSE = { 0.0f, 0.0f, 0.0f, 1.0f};
float ClearDepth = 1.0f;

float Script : STANDARDSGLOBAL
<
	string UIWidget = "none";
	string ScriptClass = "scene";
	string ScriptOrder = "postprocess";
	string ScriptOutput = "color";
	
	// We just call a script in the main technique.
	string Script = "Technique=Bloom;";

> = 0.8;

///////////////////////////////////////////////////////////
/////////////////////////////////////// Tweakables ////////
///////////////////////////////////////////////////////////

float SceneIntensity <
    string UIName = "Scene intensity";
    string UIWidget = "slider";
    float UIMin = 0.0f;
    float UIMax = 2.0f;
    float UIStep = 0.1f;
> = 0.5f;

float GlowIntensity <
    string UIName = "Glow intensity";
    string UIWidget = "slider";
    float UIMin = 0.0f;
    float UIMax = 2.0f;
    float UIStep = 0.1f;
> = 0.5f;

float HighlightThreshold <
    string UIName = "Highlight threshold";
    string UIWidget = "slider";
    float UIMin = 0.0f;
    float UIMax = 1.0f;
    float UIStep = 0.1f;
> = 0.9f;

float HighlightIntensity <
    string UIName = "Highlight intensity";
    string UIWidget = "slider";
    float UIMin = 0.0f;
    float UIMax = 10.0f;
    float UIStep = 0.1f;
> = 0.5f;

///////////////////////////////////////////////////////////
///////////////////////////// Render-to-Texture Data //////
///////////////////////////////////////////////////////////

float2 WindowSize : VIEWPORTPIXELSIZE < string UIWidget = "none"; >;
float downsampleScale = 0.25;

float BlurWidth <
    string UIName = "Blur width";
    string UIWidget = "slider";
    float UIMin = 0.0f;
    float UIMax = 10.0f;
    float UIStep = 0.5f;
> = 2.0f;

texture SceneMap : RENDERCOLORTARGET
< 
    float2 ViewportRatio = { 1.0, 1.0 };
    int MIPLEVELS = 1;
    string format = "X8R8G8B8";
>;
texture DepthMap : RENDERDEPTHSTENCILTARGET
<
    float2 ViewportRatio = { 1.0, 1.0 };
    string format = "D24S8";
>;

sampler SceneSampler = sampler_state 
{
    texture = <SceneMap>;
    AddressU  = CLAMP;        
    AddressV  = CLAMP;
    AddressW  = CLAMP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

texture DownsampleMap : RENDERCOLORTARGET
< 
    float2 ViewportRatio = { 0.25, 0.25 };
    int MIPLEVELS = 1;
    string format = "A8R8G8B8";
>;

sampler DownsampleSampler = sampler_state 
{
    texture = <DownsampleMap>;
    AddressU  = CLAMP;        
    AddressV  = CLAMP;
    AddressW  = CLAMP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

texture HBlurMap : RENDERCOLORTARGET
< 
    float2 ViewportRatio = { 0.25, 0.25 };
    int MIPLEVELS = 1;
    string format = "A8R8G8B8";
>;

sampler HBlurSampler = sampler_state 
{
    texture = <HBlurMap>;
    AddressU  = CLAMP;        
    AddressV  = CLAMP;
    AddressW  = CLAMP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

texture FinalBlurMap : RENDERCOLORTARGET
< 
    float2 ViewportRatio = { 0.25, 0.25 };
    int MIPLEVELS = 1;
    string format = "A8R8G8B8";
>;

sampler FinalBlurSampler = sampler_state 
{
    texture = <FinalBlurMap>;
    AddressU  = CLAMP;        
    AddressV  = CLAMP;
    AddressW  = CLAMP;
    MIPFILTER = NONE;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

///////////////////////////////////////////////////////////
/////////////////////////////////// data structures ///////
///////////////////////////////////////////////////////////

struct VS_OUTPUT_BLUR
{
    float4 Position   : POSITION;
    float2 TexCoord[8]: TEXCOORD0;
};

struct VS_OUTPUT
{
   	float4 Position   : POSITION;
    float2 TexCoord0  : TEXCOORD0;
    float2 TexCoord1  : TEXCOORD1;
};

struct VS_OUTPUT_DOWNSAMPLE
{
    float4 Position   : POSITION;
    float2 TexCoord[4]: TEXCOORD0;
};

////////////////////////////////////////////////////////////
////////////////////////////////// vertex shaders //////////
////////////////////////////////////////////////////////////

// generate texture coordinates to sample 4 neighbours
VS_OUTPUT_DOWNSAMPLE VS_Downsample(float4 Position : POSITION,
								   float2 TexCoord : TEXCOORD0)
{
	VS_OUTPUT_DOWNSAMPLE OUT;
	float2 texelSize = downsampleScale / WindowSize;
	float2 s = TexCoord;
	OUT.Position = Position;
	OUT.TexCoord[0] = s;
	OUT.TexCoord[1] = s + float2(2, 0)*texelSize;
	OUT.TexCoord[2] = s + float2(2, 2)*texelSize;
	OUT.TexCoord[3] = s + float2(0, 2)*texelSize;	
	return OUT;
}

// generate texcoords for blur
VS_OUTPUT_BLUR VS_Blur(float4 Position : POSITION, 
					   float2 TexCoord : TEXCOORD0,
					   uniform int nsamples,
					   uniform float2 direction
					   )
{
    VS_OUTPUT_BLUR OUT = (VS_OUTPUT_BLUR)0;
    OUT.Position = Position;
	float2 texelSize = BlurWidth / WindowSize;
    float2 s = TexCoord - texelSize*(nsamples-1)*0.5*direction;
    for(int i=0; i<nsamples; i++) {
    	OUT.TexCoord[i] = s + texelSize*i*direction;
    }
    return OUT;
}

VS_OUTPUT VS_Quad(float4 Position : POSITION, 
				  float2 TexCoord : TEXCOORD0)
{
    VS_OUTPUT OUT;
	float2 texelSize = 1.0 / WindowSize;
    OUT.Position = Position;
    // don't want bilinear filtering on original scene:
    OUT.TexCoord0 = TexCoord + texelSize*0.5;
    OUT.TexCoord1 = TexCoord + texelSize*0.5/downsampleScale;
    return OUT;
}

//////////////////////////////////////////////////////
////////////////////////////////// pixel shaders /////
//////////////////////////////////////////////////////

half luminance(half3 c)
{
	return dot( c, float3(0.3, 0.59, 0.11) );
}

// this function should be baked into a texture lookup for performance
half highlights(half3 c)
{
	return smoothstep(HighlightThreshold, 1.0, luminance(c.rgb));
}

half4 PS_Downsample(VS_OUTPUT_DOWNSAMPLE IN,
				    uniform sampler2D tex) : COLOR
{
	half4 c;
#if 0
	// sub sampling
	c = tex2D(tex, IN.TexCoord[0]);
#else
	// box filter
	c = tex2D(tex, IN.TexCoord[0]) * 0.25;
	c += tex2D(tex, IN.TexCoord[1]) * 0.25;
	c += tex2D(tex, IN.TexCoord[2]) * 0.25;
	c += tex2D(tex, IN.TexCoord[3]) * 0.25;
#endif

	// store hilights in alpha
	c.a = highlights(c.rgb);

	return c;
}

// blur filter weights
const half weights7[7] = {
	0.05,
	0.1,
	0.2,
	0.3,
	0.2,
	0.1,
	0.05,
};	

const half weights7_Central[7] = {
	0.0,
	0.0,
	0.2,
	0.6,
	0.2,
	0.0,
	0.0,
};	

// fx doesn't support variable length arrays
// otherwise we could generalize this
half4 PS_Blur7(VS_OUTPUT_BLUR IN,
			   uniform sampler2D tex,
			   uniform half weight[7]
			   ) : COLOR
{
    half4 c = 0;
    // this loop will be unrolled by compiler
    for(int i=0; i<7; i++) {
    	c += tex2D(tex, IN.TexCoord[i]) * weight[i];
   	}
    return c;
} 


half4 PS_Display(VS_OUTPUT IN,
			  uniform sampler2D tex) : COLOR
{   
	return tex2D(tex, IN.TexCoord1);
}

half4 PS_Comp(VS_OUTPUT IN,
			  uniform sampler2D sceneSampler,
			  uniform sampler2D blurredSceneSampler) : COLOR
{   
	half4 orig = tex2D(sceneSampler, IN.TexCoord0);
	half4 blur = tex2D(blurredSceneSampler, IN.TexCoord1);
	return SceneIntensity*orig + GlowIntensity*blur + HighlightIntensity*blur.a;
}  

////////////////////////////////////////////////////////////
/////////////////////////////////////// techniques /////////
////////////////////////////////////////////////////////////
technique Bloom
<
	string Script = "ClearSetDepth=ClearDepth;"
	      	"RenderColorTarget=SceneMap;"
	        "RenderDepthStencilTarget=DepthMap;"
	        	"ClearSetColor=ClearColor;"
	        	"ClearSetDepth=ClearDepth;"
   				"Clear=Color;"
				"Clear=Depth;"
	        	"ScriptSignature=color;"
	        	"ScriptExternal=;"
	        "Pass=DownSample;"
	        "Pass=GlowH;"
	        "Pass=GlowV;"
	        "Pass=FinalComp;";
>
{
    pass DownSample
    <
		string Script = "RenderColorTarget0=DownsampleMap;"
								"ClearSetColor=ClearColor;"
								"Clear=Color;"
								"Draw=Buffer;";
	>
    {
		cullmode = none;
		ZEnable = false;
		ZWriteEnable = false;		
		VertexShader = compile vs_2_0 VS_Downsample();
		PixelShader  = compile ps_2_0 PS_Downsample(SceneSampler);
    }
    pass GlowH
    <
    	string Script = "RenderColorTarget0=HBlurMap;"
	        	"ClearSetColor=ClearColor;"
	        	"Clear=Color;"
	        	"Draw=Buffer;";
	>
    {
		cullmode = none;
		ZEnable = false;
		ZWriteEnable = false;
		VertexShader = compile vs_2_0 VS_Blur(7, float2(1, 0));
//		PixelShader  = compile ps_2_0 PS_Blur7(SceneSampler, weights7);
		PixelShader  = compile ps_2_0 PS_Blur7(DownsampleSampler, weights7);
	}
	pass GlowV
	<
		string Script = "RenderColorTarget0=FinalBlurMap;"
	        						"ClearSetColor=ClearColor;"
	        						"Clear=color;"
	        						"Draw=Buffer;";
	>
	{
		cullmode = none;
		ZEnable = false;
		ZWriteEnable = false;
		VertexShader = compile vs_2_0 VS_Blur(7, float2(0, 1));
		PixelShader  = compile ps_2_0 PS_Blur7(HBlurSampler, weights7);
	}
	pass FinalComp
	<
		string Script = "RenderColorTarget=;"
	         					"RenderDepthStencilTarget=;"
	        					"Draw=Buffer;";        	
	>
	{
		cullmode = none;
		ZEnable = false;
		ZWriteEnable = false;
		VertexShader = compile vs_2_0 VS_Quad();
		PixelShader  = compile ps_2_0 PS_Comp(SceneSampler, FinalBlurSampler);
	}
}
