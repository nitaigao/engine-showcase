//
// 4 bone only version of the "Skinned Mesh Effect" file, based
// Copyright (c) 2000-2002 Microsoft Corporation. All rights reserved.
//


// Matrix Pallette
static const int MAX_MATRICES = 26;
float4x3    mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY : register(c0);
float4x4    mViewProj : VIEWPROJECTION : register(c78); // have it directly follow above 26 (26*3 == 78)
float4		lhtDir : register(c82) = {0.0f, 0.0f, -1.0f, 1.0f} ; 
//float4		lightDiffuse = {0.6f, 0.6f, 0.6f, 1.0f}; // Light Diffuse
float4		MaterialAmbient : MATERIALAMBIENT : register(c83) = {0.1f, 0.1f, 0.1f, 1.0f} ; 
float4		MaterialDiffuse : MATERIALDIFFUSE : register(c84) = {0.8f, 0.8f, 0.8f, 1.0f} ; 

///////////////////////////////////////////////////////
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
    float3  Normal          : NORMAL;
    float3  Tex0            : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4  Pos     : POSITION;
    float4  Diffuse : COLOR;
    float2  Tex0    : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};

sampler TextureSampler : register(s0);


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT PShade( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse and add specular
    Output.RGBColor = tex2D(TextureSampler, In.Tex0) * In.Diffuse; //+ In.specCol

    return Output;
}


float3 Diffuse(float3 Normal)
{
    float CosTheta;
    
    // N.L Clamped
    CosTheta = max(0.0f, dot(Normal, lhtDir.xyz));
       
    // propogate scalar result to vector
    return (CosTheta);
}


VS_OUTPUT VShade(VS_INPUT i)
{
    VS_OUTPUT   o;
    float3      Pos = 0.0f;
    float3      Normal = 0.0f;    
    float       LastWeight = 0.0f;
     
    // Compensate for lack of UBYTE4 on Geforce3
    int4 IndexVector = D3DCOLORtoUBYTE4(i.BlendIndices);
    
    // cast the vectors to arrays for use in the for loop below
    float BlendWeightsArray[4] = (float[4])i.BlendWeights;
    int   IndexArray[4] = (int[4])IndexVector;    
    
    // calculate the pos/normal using the "normal" weights 
    //        and accumulate the weights to calculate the last weight
    for (int iBone = 0; iBone < 3; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(i.Pos, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        Normal += mul(i.Normal, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 

    // Now that we have the calculated weight, add in the final influence
    Pos += (mul(i.Pos, mWorldMatrixArray[IndexArray[3]]) * LastWeight);
    Normal += (mul(i.Normal, mWorldMatrixArray[IndexArray[3]]) * LastWeight); 
    
    // transform position from world space into view and then projection space
    o.Pos = mul(float4(Pos.xyz, 1.0f), mViewProj);

    // normalize normals
    Normal = normalize(Normal);

    // Shade (Ambient + etc.)
    o.Diffuse.xyz = MaterialAmbient.xyz + Diffuse(Normal) * MaterialDiffuse.xyz;
    o.Diffuse.w = 1.0f;

    // copy the input texture coordinate through
    o.Tex0  = i.Tex0.xy;

    return o;
}
