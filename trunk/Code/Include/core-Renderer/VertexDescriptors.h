/// @file   core-Renderer/VertexDescriptors.h
/// @brief  vertex descriptors used to define what a vertex structure passed to a vertex shader comprises
#pragma once


///////////////////////////////////////////////////////////////////////////////

struct VertexDescriptor
{
   unsigned short    m_stream;     // Stream index
   unsigned short    m_offset;     // Offset in the stream in bytes
   unsigned char     m_type;       // Data type
   unsigned char     m_method;     // Processing method
   unsigned char     m_usage;      // Semantics
   unsigned char     m_usageIndex; // Semantic index
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A macro that ends a vertex descriptor declaration
 */
#define VERTEX_DESCRIPTOR_END() {0xFF,0,VTXTYPE_UNUSED,0,0,0}

///////////////////////////////////////////////////////////////////////////////

/**
 * Data type definition ( what goes to m_type filed )
 */
enum VertexType
{
   VTXTYPE_FLOAT1    =  0,  // 1D float expanded to (value, 0., 0., 1.)
   VTXTYPE_FLOAT2    =  1,  // 2D float expanded to (value, value, 0., 1.)
   VTXTYPE_FLOAT3    =  2,  // 3D float expanded to (value, value, value, 1.)
   VTXTYPE_FLOAT4    =  3,  // 4D float
   VTXTYPE_D3DCOLOR  =  4,  // 4D packed unsigned bytes mapped to 0. to 1. range
   // Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A)
   VTXTYPE_UBYTE4    =  5,  // 4D unsigned byte
   VTXTYPE_SHORT2    =  6,  // 2D signed short expanded to (value, value, 0., 1.)
   VTXTYPE_SHORT4    =  7,  // 4D signed short

   // The following types are valid only with vertex shaders >= 2.0


   VTXTYPE_UBYTE4N   =  8,  // Each of 4 bytes is normalized by dividing to 255.0
   VTXTYPE_SHORT2N   =  9,  // 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
   VTXTYPE_SHORT4N   = 10,  // 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
   VTXTYPE_USHORT2N  = 11,  // 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)
   VTXTYPE_USHORT4N  = 12,  // 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)
   VTXTYPE_UDEC3     = 13,  // 3D unsigned 10 10 10 format expanded to (value, value, value, 1)
   VTXTYPE_DEC3N     = 14,  // 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
   VTXTYPE_FLOAT16_2 = 15,  // Two 16-bit floating point values, expanded to (value, value, 0, 1)
   VTXTYPE_FLOAT16_4 = 16,  // Four 16-bit floating point values
   VTXTYPE_UNUSED    = 17,  // When the type field in a decl is unused.
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Processing method definition ( what goes to m_method )
 */
enum VertexProcessingMethod
{
   VTXMETHOD_DEFAULT = 0,
   VTXMETHOD_PARTIALU,
   VTXMETHOD_PARTIALV,
   VTXMETHOD_CROSSUV,    // Normal
   VTXMETHOD_UV,
   VTXMETHOD_LOOKUP,               // Lookup a displacement map
   VTXMETHOD_LOOKUPPRESAMPLED,     // Lookup a pre-sampled displacement map
};

///////////////////////////////////////////////////////////////////////////////

enum VertexUsage
{
   VTXUSAGE_POSITION = 0,
   VTXUSAGE_BLENDWEIGHT,   // 1
   VTXUSAGE_BLENDINDICES,  // 2
   VTXUSAGE_NORMAL,        // 3
   VTXUSAGE_PSIZE,         // 4
   VTXUSAGE_TEXCOORD,      // 5
   VTXUSAGE_TANGENT,       // 6
   VTXUSAGE_BINORMAL,      // 7
   VTXUSAGE_TESSFACTOR,    // 8
   VTXUSAGE_POSITIONT,     // 9
   VTXUSAGE_COLOR,         // 10
   VTXUSAGE_FOG,           // 11
   VTXUSAGE_DEPTH,         // 12
   VTXUSAGE_SAMPLE,        // 13
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Flexible vertex format bits
 */
#define FVF_RESERVED0        0x001
#define FVF_POSITION_MASK    0x400E
#define FVF_XYZ              0x002
#define FVF_XYZRHW           0x004
#define FVF_XYZB1            0x006
#define FVF_XYZB2            0x008
#define FVF_XYZB3            0x00a
#define FVF_XYZB4            0x00c
#define FVF_XYZB5            0x00e
#define FVF_XYZW             0x4002

#define FVF_NORMAL           0x010
#define FVF_PSIZE            0x020
#define FVF_DIFFUSE          0x040
#define FVF_SPECULAR         0x080

#define FVF_TEXCOUNT_MASK    0xf00
#define FVF_TEXCOUNT_SHIFT   8
#define FVF_TEX0             0x000
#define FVF_TEX1             0x100
#define FVF_TEX2             0x200
#define FVF_TEX3             0x300
#define FVF_TEX4             0x400
#define FVF_TEX5             0x500
#define FVF_TEX6             0x600
#define FVF_TEX7             0x700
#define FVF_TEX8             0x800

#define FVF_LASTBETA_UBYTE4   0x1000
#define FVF_LASTBETA_D3DCOLOR 0x8000

#define FVF_RESERVED2         0x6000  // 2 reserved bits

///////////////////////////////////////////////////////////////////////////////
