#include "core-Renderer\BatchingStrategy.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Material.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

bool BatchComparator::operator()(AbstractGraphicalNode* lhs, 
                                 AbstractGraphicalNode* rhs) const
{
   Material& mat1 = lhs->getMaterial();
   Material& mat2 = rhs->getMaterial();

   return mat1.getIndex() < mat2.getIndex();
}

///////////////////////////////////////////////////////////////////////////////

void DistanceComparator::setReferencePoint(const D3DXVECTOR3& pt)
{
   m_referencePoint = pt;
}

///////////////////////////////////////////////////////////////////////////////

bool DistanceComparator::operator()(AbstractGraphicalNode* lhs, 
                                    AbstractGraphicalNode* rhs) const
{
   const D3DXMATRIX& mtx1 = lhs->getGlobalMtx();
   const D3DXMATRIX& mtx2 = rhs->getGlobalMtx();

   return calcDistanceTo(mtx1._41, mtx1._42, mtx1._43) > calcDistanceTo(mtx2._41, mtx2._42, mtx2._43);
}

///////////////////////////////////////////////////////////////////////////////

double DistanceComparator::calcDistanceTo(float x, float y, float z) const
{
   double dx = x - m_referencePoint.x;
   double dy = y - m_referencePoint.y;
   double dz = z - m_referencePoint.z;
   return dx*dx + dy*dy + dz*dz;
}

///////////////////////////////////////////////////////////////////////////////
