//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4 g_MaterialAmbientColor;      // Material's ambient color
float4 g_MaterialDiffuseColor;      // Material's diffuse color

float3 g_LightDir[3];               // Light's direction in world space
float4 g_LightDiffuse[3];           // Light's diffuse color
float4 g_LightAmbient;              // Light's ambient color

texture g_MeshTexture;              // Color texture for mesh

float4x4 g_mWorldView;              // Object world matrix * view matrix 
float4x4 g_mProjection;             // Projection matrix



//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position     : POSITION;   // vertex position 
    float4 Diffuse      : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV    : TEXCOORD0;  // vertex texture coords 
    float3 Normal       : TEXCOORD1;
    float3 ViewPosition : TEXCOORD2;
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;
    
    // Transform the position from object space to homogeneous projection space   
    Output.ViewPosition = mul(vPos, g_mWorldView);
    Output.Position = mul( float4(Output.ViewPosition, 1.0f), g_mProjection);

    // Transform normal to view space
    Output.Normal = mul(vNormal, (float3x3)g_mWorldView);
    
    // Compute simple directional lighting equation
    Output.Diffuse = g_MaterialDiffuseColor;

    // Just copy the texture coordinate through
    Output.TextureUV = vTexCoord0; 
    
    return Output;    
}


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 Color : COLOR0;       // Pixel color    
    float4 Normal : COLOR1;      // Normal
    float4 Position : COLOR2;    // Position
};


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS(VS_OUTPUT In) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    Output.Color = tex2D(MeshTextureSampler, In.TextureUV) * In.Diffuse;
        
    // calculate normal
    Output.Normal = float4(normalize(In.Normal), 1.0f);
    
    // calculate positon
    Output.Position = float4(In.ViewPosition, 1.0f);

    return Output;
}


//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderSceneNoLight
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS();
        PixelShader  = compile ps_2_0 RenderScenePS();
        ZEnable = true;
    }
}
