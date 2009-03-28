#pragma once

#include <set>
#include <d3dx9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalNode;
class Material;

///////////////////////////////////////////////////////////////////////////////

class BatchComparator
{
public:
   bool operator()(AbstractGraphicalNode* lhs, AbstractGraphicalNode* rhs) const;
};

///////////////////////////////////////////////////////////////////////////////

class DistanceComparator
{
private:
   D3DXVECTOR3 m_referencePoint;

public:
   void setReferencePoint(const D3DXVECTOR3& pt);

   bool operator()(AbstractGraphicalNode* lhs, AbstractGraphicalNode* rhs) const;

private:
   inline double calcDistanceTo(float x, float y, float z) const;
};

///////////////////////////////////////////////////////////////////////////////
