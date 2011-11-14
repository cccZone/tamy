// VertexShaderOutput.vsh

//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position     : POSITION;     // vertex position 
    float2 TextureUV    : TEXCOORD0;    // vertex texture coords 
    float3 Normal       : TEXCOORD1;    // vertex normal
    float3 ViewPosition : TEXCOORD2;
};
