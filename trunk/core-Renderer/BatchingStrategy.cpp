#include "core-Renderer\BatchingStrategy.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Material.h"
#include <cassert>
#include <math.h>

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
   static double maxDist = 25000000;
   assert(lhs != NULL);
   assert(rhs != NULL);

   Material& mat1 = lhs->getMaterial();
   Material& mat2 = rhs->getMaterial();

   double id1, id2;
   
   if (mat1.isTransparent())
   {
      id1 = maxDist - calcDistance(lhs->getGlobalMtx());
   }
   else
   {
      id1 = -(double)(mat1.getIndex());
   }

   if (mat2.isTransparent())
   {
      id2 = maxDist - calcDistance(rhs->getGlobalMtx());
   }
   else
   {
      id2 = -(double)(mat2.getIndex());
   }
   
   return id1 < id2;
}

///////////////////////////////////////////////////////////////////////////////

double BatchComparator::calcDistance(const D3DXMATRIX& mtx) const
{
   double dx = (mtx._41 - m_cameraPos.x);
   double dy = (mtx._42 - m_cameraPos.y);
   double dz = (mtx._43 - m_cameraPos.z);
   return dx*dx + dy*dy + dz*dz;
}

///////////////////////////////////////////////////////////////////////////////
