// StaticGeometry.vsh
#include "Renderer/Shaders/Lib/VertexShaderOutput.tfsh"

//--------------------------------------------------------------------------------------
// Global constants
//--------------------------------------------------------------------------------------
float4x4 g_mWorldView;              // Object world matrix * view matrix 
float4x4 g_mProjection;             // Projection matrix

//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT main( float4 vPos : POSITION, 
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

    // Just copy the texture coordinate through
    Output.TextureUV = vTexCoord0;
    
    return Output;    
}
