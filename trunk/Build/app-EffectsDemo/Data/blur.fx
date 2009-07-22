///////////////////////////////////////////////////////////////////////////////
// Effect File Variables
///////////////////////////////////////////////////////////////////////////////

texture texturePrev3; 
texture texturePrev2; 
texture texturePrev1; 
texture texturePrev0; 
texture textureNow; 

sampler SamplerNow = sampler_state
{
    Texture   = (textureNow);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler SamplerPrev0 = sampler_state
{
    Texture   = (texturePrev0);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler SamplerPrev1 = sampler_state
{
    Texture   = (texturePrev1);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler SamplerPrev2 = sampler_state
{
    Texture   = (texturePrev2);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler SamplerPrev3 = sampler_state
{
    Texture   = (texturePrev3);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

///////////////////////////////////////////////////////////////////////////////
// Vertex Definitions
///////////////////////////////////////////////////////////////////////////////

struct VS_OUTPUT
{
    float4 hposition : POSITION;
	 float2 texture0  : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 color : COLOR;
};


///////////////////////////////////////////////////////////////////////////////
// Pixel Shaders
///////////////////////////////////////////////////////////////////////////////

PS_OUTPUT blurPS( VS_OUTPUT IN )
{
   PS_OUTPUT OUT;
   float4 col;
   float2 texCoord = IN.texture0;
   
   OUT.color = tex2D(SamplerNow, texCoord) * 0.5;
   OUT.color.a = 0.8;
   
   texCoord.x = 0.5 + (texCoord.x - 0.5) * 0.99;
   texCoord.y = 0.5 + (texCoord.y - 0.5) * 0.99;
   col = tex2D(SamplerPrev0, texCoord) * 0.2;
   col.a = 0.6;
   OUT.color += col;
   
   texCoord.x = 0.5 + (texCoord.x - 0.5) * 0.96;
   texCoord.y = 0.5 + (texCoord.y - 0.5) * 0.96;
   col = tex2D(SamplerPrev1, texCoord) * 0.2;
   col.a = 0.4;
   OUT.color += col;
   
   texCoord.x = 0.5 + (texCoord.x - 0.5) * 0.90;
   texCoord.y = 0.5 + (texCoord.y - 0.5) * 0.90;
   col = tex2D(SamplerPrev2, texCoord) * 0.2;
   col.a = 0.2;
   OUT.color += col;
   
   texCoord.x = 0.5 + (texCoord.x - 0.5) * 0.85;
   texCoord.y = 0.5 + (texCoord.y - 0.5) * 0.85;
   col = tex2D(SamplerPrev3, texCoord) * 0.5;
   col.a = 0.1;
   OUT.color += col;

   return OUT;
}


///////////////////////////////////////////////////////////////////////////////
// techniques
///////////////////////////////////////////////////////////////////////////////

technique render
{
    pass Pass0
    {
		Lighting = FALSE;

		VertexShader = null;
		PixelShader  = compile ps_2_0 blurPS();
    }
}

