#pragma once

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

template<class T>
class DistanceComparator
{
private:
   D3DXVECTOR3 m_referencePoint;

public:
   void setReferencePoint(const D3DXVECTOR3& pt)
   {
      m_referencePoint = pt;
   }

   bool operator()(T* lhs, T* rhs) const
   {
      const D3DXMATRIX& mtx1 = lhs->getGlobalMtx();
      const D3DXMATRIX& mtx2 = rhs->getGlobalMtx();

      return calcDistanceTo(mtx1._41, mtx1._42, mtx1._43) > calcDistanceTo(mtx2._41, mtx2._42, mtx2._43);
   }

private:
   double calcDistanceTo(float x, float y, float z) const
   {
      double dx = x - m_referencePoint.x;
      double dy = y - m_referencePoint.y;
      double dz = z - m_referencePoint.z;
      return dx*dx + dy*dy + dz*dz;
   }
};

///////////////////////////////////////////////////////////////////////////////
