#include "core-Renderer\VertexDescriptions.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ENUM( VertexDescId );
   ENUM_VAL( VDI_SIMPLE );
   ENUM_VAL( VDI_SIMPLE_SKINNING );
   ENUM_VAL( VDI_SIMPLE_SCREENSPACE );
END_ENUM();

///////////////////////////////////////////////////////////////////////////////

D3DVERTEXELEMENT9 SimpleDesc[] = 
{
   {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0}, // stream 0, position
   {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},   // stream 0, normal
   {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},   // stream 0, texture coords
   D3DDECL_END()
};

D3DVERTEXELEMENT9 SimpleSkinningDesc[] = 
{
   {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0}, // stream 0, position
   {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},   // stream 0, normal
   {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},   // stream 0, texture coords
   {1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1}, // stream 1, blend weights (up to 4 influences)
   {1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},   // stream 1, blend indices (up to 4 influences)
   D3DDECL_END()
};

D3DVERTEXELEMENT9 SimpleScreenspaceDesc[] = 
{
   {0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0}, // stream 0, position
   {0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},   // stream 0, texture coords
   D3DDECL_END()
};

D3DVERTEXELEMENT9* g_vertexDescriptions[] = {
   SimpleDesc,
   SimpleSkinningDesc,
   SimpleScreenspaceDesc,
};

///////////////////////////////////////////////////////////////////////////////
