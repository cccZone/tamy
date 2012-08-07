#include "core-Renderer\VertexDescriptions.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ENUM( VertexDescId );
   ENUM_VAL( VDI_SIMPLE );
   ENUM_VAL( VDI_SIMPLE_SKINNING );
   ENUM_VAL( VDI_SIMPLE_SCREENSPACE );
   ENUM_VAL( VDI_DEBUG_GEOMETRY );
   ENUM_VAL( VDI_LIGHT );
END_ENUM();

///////////////////////////////////////////////////////////////////////////////

VertexDescriptor SimpleDesc[] = 
{
   {0, 0, VTXTYPE_FLOAT3, VTXMETHOD_DEFAULT, VTXUSAGE_POSITION, 0}, // stream 0, position
   {0, 12, VTXTYPE_FLOAT3, VTXMETHOD_DEFAULT, VTXUSAGE_NORMAL, 0},   // stream 0, normal
   {0, 24, VTXTYPE_FLOAT3, VTXMETHOD_DEFAULT, VTXUSAGE_TANGENT, 0},   // stream 0, tangent
   {0, 36, VTXTYPE_FLOAT2, VTXMETHOD_DEFAULT, VTXUSAGE_TEXCOORD, 0},   // stream 0, texture coords
   VERTEX_DESCRIPTOR_END()
};

VertexDescriptor SimpleSkinningDesc[] = 
{
   {0, 0, VTXTYPE_FLOAT3, VTXMETHOD_DEFAULT, VTXUSAGE_POSITION, 0}, // stream 0, position
   {0, 12, VTXTYPE_FLOAT3, VTXMETHOD_DEFAULT, VTXUSAGE_NORMAL, 0},   // stream 0, normal
   {0, 24, VTXTYPE_FLOAT3, VTXMETHOD_DEFAULT, VTXUSAGE_TANGENT, 0},   // stream 0, tangent
   {0, 36, VTXTYPE_FLOAT2, VTXMETHOD_DEFAULT, VTXUSAGE_TEXCOORD, 0},   // stream 0, texture coords
   {1, 0, VTXTYPE_FLOAT4, VTXMETHOD_DEFAULT, VTXUSAGE_TEXCOORD, 1}, // stream 1, blend weights (up to 4 influences)
   {1, 16, VTXTYPE_FLOAT4, VTXMETHOD_DEFAULT, VTXUSAGE_BLENDINDICES, 0},   // stream 1, blend indices (up to 4 influences)
   VERTEX_DESCRIPTOR_END()
};

VertexDescriptor SimpleScreenspaceDesc[] = 
{
   {0, 0, VTXTYPE_FLOAT2, VTXMETHOD_DEFAULT, VTXUSAGE_POSITION, 0}, // stream 0, position
   {0, 8, VTXTYPE_FLOAT2, VTXMETHOD_DEFAULT, VTXUSAGE_TEXCOORD, 0},   // stream 0, texture coords
   VERTEX_DESCRIPTOR_END()
};

VertexDescriptor DebugGeometryDesc[] = 
{
   {0, 0, VTXTYPE_FLOAT3, VTXMETHOD_DEFAULT, VTXUSAGE_POSITION, 0}, // stream 0, position
   {0, 12, VTXTYPE_D3DCOLOR, VTXMETHOD_DEFAULT, VTXUSAGE_COLOR, 0}, // stream 0, color
   VERTEX_DESCRIPTOR_END()
};

VertexDescriptor LightGeometryDesc[] = 
{
   {0, 0, VTXTYPE_FLOAT3, VTXMETHOD_DEFAULT, VTXUSAGE_POSITION, 0}, // stream 0, position
   VERTEX_DESCRIPTOR_END()
};

VertexDescriptor* g_vertexDescriptions[] = {
   SimpleDesc,
   SimpleSkinningDesc,
   SimpleScreenspaceDesc,
   DebugGeometryDesc,
   LightGeometryDesc,
};

///////////////////////////////////////////////////////////////////////////////
