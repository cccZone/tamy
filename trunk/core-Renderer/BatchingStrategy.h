#pragma once

#include <set>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalNode;

///////////////////////////////////////////////////////////////////////////////

class BatchComparator
{
private:
   D3DXVECTOR3 m_cameraPos;

public:
   BatchComparator();
   BatchComparator(const D3DXVECTOR3& cameraPos);

   bool operator()(AbstractGraphicalNode* lhs, AbstractGraphicalNode* rhs) const;

private:
   inline DWORD calcDistance(const D3DXMATRIX& mtx) const;
};

///////////////////////////////////////////////////////////////////////////////

typedef std::multiset<AbstractGraphicalNode*, BatchComparator> BatchedNodes;

///////////////////////////////////////////////////////////////////////////////
