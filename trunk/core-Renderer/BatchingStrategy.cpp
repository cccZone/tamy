#include "BatchingStrategy.h"
#include "AbstractGraphicalNode.h"
#include "Material.h"
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

BatchComparator::BatchComparator()
      : m_cameraPos(D3DXVECTOR3(0, 0, 0))
{
}

///////////////////////////////////////////////////////////////////////////////

BatchComparator::BatchComparator(const D3DXVECTOR3& cameraPos)
      : m_cameraPos(cameraPos)
{
}

///////////////////////////////////////////////////////////////////////////////

bool BatchComparator::operator()(AbstractGraphicalNode* lhs, 
                                 AbstractGraphicalNode* rhs) const
{
   assert(lhs != NULL);
   assert(rhs != NULL);

   Material& mat1 = lhs->getMaterial();
   Material& mat2 = rhs->getMaterial();

   DWORD id1, id2;
   
   if (mat1.isTransparent())
   {
      id1 = (0xffffffff - calcDistance(lhs->getGlobalMtx())) << 16;
   }
   else
   {
      id1 = mat1.getIndex();
   }

   if (mat2.isTransparent())
   {
      id2 = (0xffffffff - calcDistance(rhs->getGlobalMtx())) << 16;
   }
   else
   {
      id2 = mat2.getIndex();
   }
   
   return id1 < id2;
}

///////////////////////////////////////////////////////////////////////////////

DWORD BatchComparator::calcDistance(const D3DXMATRIX& mtx) const
{
   long dx = (long)(mtx._41 - m_cameraPos.x);
   long dy = (long)(mtx._42 - m_cameraPos.y);
   long dz = (long)(mtx._43 - m_cameraPos.z);
   return dx*dx + dy*dy + dz*dz;
}

///////////////////////////////////////////////////////////////////////////////
