#ifndef _DISTANCE_COMPARATOR_H
#error "This file can only be included from DistanceComparator.h"
#else

///////////////////////////////////////////////////////////////////////////////

void DistanceComparator::setReferencePoint(const D3DXVECTOR3& pt)
{
   m_referencePoint = pt;
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

#endif // _DISTANCE_COMPARATOR_H
