
static const int MAX_MATRICES = 48;
float4x3    g_mLocalMatrixArray[MAX_MATRICES] : WorldMatrixArray : register(c0);

float4x4    g_mWorld;
float4x4    g_mWorldViewProj;
float4x4    g_mWorldInvTranspose;

// support for one dir light
float4 g_vLightDir : Direction  = {0.0f, 1.0f, 0.0f, 1.0f}; // world space
float4 g_cLightColor : Diffuse = {1.0f, 1.0f, 1.0f, 1.0f};

// support for diffuse mat
float4 g_cAmbientColor	: MaterialAmbient = {0.0f, 0.0f, 0.0f, 1.0f};
float4 g_cDiffuseColor	: MaterialDiffuse = {1.0f, 1.0f, 1.0f, 1.0f};

// Support for one diffuse tex at time, no shadows
sampler g_sSamplerZero : register(s0);   


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


VS_OUTPUT mainVS(VS_INPUT i)
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
    // and accumulate the weights to calculate the last weight
    for (int iBone = 0; iBone < 3; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(i.Pos, g_mLocalMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        Normal += mul(i.Normal, g_mLocalMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 

    // Now that we have the calculated weight, add in the final influence
    Pos += (mul(i.Pos, g_mLocalMatrixArray[IndexArray[3]]) * LastWeight);
    Normal += (mul(i.Normal, g_mLocalMatrixArray[IndexArray[3]]) * LastWeight); 
    
    // transform position from world space into view and then projection space
    o.Pos = mul(float4(Pos.xyz, 1.0f), g_mWorldViewProj);

    // normalize normals
    Normal = mul(float4(Normal.xyz, 1.0f), g_mWorldInvTranspose);
    Normal = normalize(Normal);

    // Shade (Ambient + etc.)
    float4 diffuseLight = max(0.0f, dot(Normal, g_vLightDir.xyz)) * g_cLightColor;
 
    o.Diffuse.xyz = g_cAmbientColor.xyz + ( diffuseLight * g_cDiffuseColor.xyz );
    o.Diffuse.w = 1.0f;

    // copy the input texture coordinate through
    o.Tex0  = i.Tex0.xy;

    return o;
}


PS_OUTPUT mainPS( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output;

    Output.RGBColor = tex2D(g_sSamplerZero, In.Tex0) * In.Diffuse; 

    return Output;
}
