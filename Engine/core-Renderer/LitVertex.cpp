#include "core-Renderer\LitVertex.h"
#include "core\MatrixWriter.h"


///////////////////////////////////////////////////////////////////////////////

DWORD LitVertex::FVF = D3DFVF_XYZB3 | D3DFVF_NORMAL | D3DFVF_TEX1;

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& stream, const LitVertex& vertex)
{
   stream << "[" << vertex.m_coords << "; " <<
             "[" << vertex.m_weight0 << ", " << vertex.m_weight1 << ", " << vertex.m_weight2 << "]; " <<
             vertex.m_normal << "; " <<
             "[" << vertex.m_texCoords[0] << ", " << vertex.m_texCoords[1] << "]]]";

   return stream;
}

///////////////////////////////////////////////////////////////////////////////
