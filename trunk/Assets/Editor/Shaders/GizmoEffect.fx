float4x4 g_mWorldViewProj;

struct VS_OUTPUT
{
    float4 pos   : POSITION;
    float4 color : COLOR;
};

VS_OUTPUT vertexShader(float4 inPos : POSITION, float4 color : COLOR)
{
   VS_OUTPUT output = (VS_OUTPUT)0;
   
   output.pos = mul(inPos, g_mWorldViewProj);
   output.color = color;
   
   return output;
}

struct PS_OUTPUT
{
    float4 color : COLOR0;   
};

PS_OUTPUT pixelShader(VS_OUTPUT vsIn)
{
   PS_OUTPUT output = (PS_OUTPUT)0;
   
   output.color = vsIn.color;
   
   return output;
}

technique tec0
{
   pass P0
   {
      Lighting = false;
      ZEnable = false;

      VertexShader = compile vs_2_0 vertexShader();
      PixelShader  = compile ps_2_0 pixelShader();
   }
}
