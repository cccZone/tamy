///////////////////////////////////////////////////////////////////////////////
// Effect File Variables
///////////////////////////////////////////////////////////////////////////////

texture inputTexture; 
float time = 1.0;

sampler Sampler = sampler_state
{
    Texture   = (inputTexture);
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

PS_OUTPUT dizzyPS( VS_OUTPUT IN )
{
   PS_OUTPUT OUT;
   
   float2 tex;
   float multiplier = 0.3;
   float dt = 0.5;
   
   tex.y = IN.texture0.y + (sin(time)*multiplier);
   tex.x = IN.texture0.x + (cos(time)*multiplier);
   OUT.color = tex2D(Sampler, tex);
   
   tex.y = IN.texture0.y + (sin(time + dt)*multiplier);
   tex.x = IN.texture0.x + (cos(time + dt)*multiplier);
   OUT.color += tex2D(Sampler, tex);
   
   tex.y = IN.texture0.y + (sin(time + 2*dt)*multiplier);
   tex.x = IN.texture0.x + (cos(time + 2*dt)*multiplier);
   OUT.color += tex2D(Sampler, tex);
   
   tex.y = IN.texture0.y + (sin(time + 3*dt)*multiplier);
   tex.x = IN.texture0.x + (cos(time + 3*dt)*multiplier);
   OUT.color += tex2D(Sampler, tex);
   
   tex.y = IN.texture0.y + (sin(time + 4*dt)*multiplier);
   tex.x = IN.texture0.x + (cos(time + 4*dt)*multiplier);
   OUT.color += tex2D(Sampler, tex);
   
   OUT.color = OUT.color * 0.25;
   	
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

		Sampler[0] = (Sampler);

		VertexShader = null;
		PixelShader  = compile ps_2_0 dizzyPS();
    }
}

