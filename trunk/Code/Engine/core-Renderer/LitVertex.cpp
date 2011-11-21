#include "core-Renderer\LitVertex.h"
#include "core\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

DWORD LitVertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

///////////////////////////////////////////////////////////////////////////////

DWORD VertexWeight::FVF = D3DFVF_DIFFUSE | D3DFVF_SPECULAR;

///////////////////////////////////////////////////////////////////////////////

Serializer& operator<<( Serializer& serializer, LitVertex& vtx )
{
   serializer << vtx.m_coords;
   serializer << vtx.m_normal;
   serializer << vtx.m_textureCoords;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////

Serializer& operator<<( Serializer& serializer, VertexWeight& weight )
{
   serializer << weight.m_weights;
   serializer << weight.m_indices;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& stream, const LitVertex& vertex)
{
   stream << "[" << vertex.m_coords << "; " << vertex.m_normal << "]";

   return stream;
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& stream, const VertexWeight& vertex)
{
   stream << "[" << vertex.m_indices << "; " << vertex.m_weights << "]";
   return stream;
}

///////////////////////////////////////////////////////////////////////////////
