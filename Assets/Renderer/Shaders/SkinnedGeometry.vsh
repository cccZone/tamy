//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position     : POSITION;							// vertex position 
	float2 TextureUV    : TEXCOORD0;
    float3 Normal       : TEXCOORD1;
    float3 ViewPosition : TEXCOORD2;
};

//--------------------------------------------------------------------------------------
// Global constants
//--------------------------------------------------------------------------------------
float4x4 g_mSkinningMatrices[62] : WORLDMATRIXARRAY;		// Skinning matrices
float4x4 g_mProjection;										// Projection matrix

//--------------------------------------------------------------------------------------
// Shader main function
//--------------------------------------------------------------------------------------
VS_OUTPUT main( float4 vPos : POSITION, 
                float3 vNormal : NORMAL,
				float2 vTexCoord0 : TEXCOORD0,
                float3 vBlendWeights : TEXCOORD1,
				float4 vBlendIndices : BLENDINDICES )
{
    VS_OUTPUT Output = (VS_OUTPUT)0;

    float3 tempPos = vBlendWeights.x * mul(vPos, g_mSkinningMatrices[vBlendIndices.x]);
	float3 tempNormal = vBlendWeights.x * mul(vNormal, g_mSkinningMatrices[vBlendIndices.x]);

	if ( vBlendIndices.y >= 0 )
	{
		tempPos += vBlendWeights.y * mul(vPos, g_mSkinningMatrices[vBlendIndices.y]);
		tempNormal += vBlendWeights.y * mul(vNormal, g_mSkinningMatrices[vBlendIndices.y]);
	}

	if ( vBlendIndices.z >= 0 )
	{
		tempPos += vBlendWeights.z * mul(vPos, g_mSkinningMatrices[vBlendIndices.z]);
		tempNormal += vBlendWeights.z * mul(vNormal, g_mSkinningMatrices[vBlendIndices.z]);
	}

	if ( vBlendIndices.w >= 0 )
	{
		float lastWeight = ( 1 - vBlendWeights.x - vBlendWeights.y - vBlendWeights.z );
		tempPos += lastWeight * mul(vPos, g_mSkinningMatrices[vBlendIndices.w]);
		tempNormal += lastWeight * mul(vNormal, g_mSkinningMatrices[vBlendIndices.w]);
	}

	Output.ViewPosition = tempPos;
	Output.Normal = tempNormal;

	// Transform the position from object space to homogeneous projection space
    Output.Position = mul( float4(Output.ViewPosition, 1.0f), g_mProjection);
    
	// Just copy the texture coordinate through
    Output.TextureUV = vTexCoord0;
	
    return Output;    
}
