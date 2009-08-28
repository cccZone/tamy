#ifndef _DISTANCE_COMPARATOR_H
#define _DISTANCE_COMPARATOR_H

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Renderable;

///////////////////////////////////////////////////////////////////////////////

/**
 * A helper to the GraphicalNodesProcessor class that allows
 * to sort renderables based on their distance from a specified
 * point in space
 */
class DistanceComparator
{
private:
   D3DXVECTOR3 m_referencePoint;

public:
   inline void setReferencePoint(const D3DXVECTOR3& pt);

   bool operator()(Renderable* lhs, Renderable* rhs) const;

private:
   inline double calcDistanceTo(float x, float y, float z) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\DistanceComparator.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _DISTANCE_COMPARATOR_H
