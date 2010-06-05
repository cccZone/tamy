float4x4 g_mWorldViewProj;
float4   g_ptr;

struct VS_OUTPUT
{
    float4 pos   : POSITION;
};

VS_OUTPUT vertexShader( float4 inPos : POSITION )
{
   VS_OUTPUT output = (VS_OUTPUT)0;

   output.pos = mul(inPos, g_mWorldViewProj);
  
   return output;
}

struct PS_OUTPUT
{
    float4 color : COLOR0;   
};

PS_OUTPUT pixelShader(VS_OUTPUT vsIn)
{
   PS_OUTPUT output = (PS_OUTPUT)0;
   
   output.color = g_ptr;
   
   return output;
}

technique tec0
{
   pass P0
   {
      Lighting = false;
      ZEnable = true;
      ZWriteEnable = true;
      AlphaBlendEnable=false;
		
      VertexShader = compile vs_2_0 vertexShader();
      PixelShader  = compile ps_2_0 pixelShader();
   }
}
       