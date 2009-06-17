#pragma once

#include <d3dx9.h>
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

struct Triangle
{
private:
   D3DXVECTOR3 v[3];
   D3DXVECTOR3 e[3];
   D3DXVECTOR3 en[3];

public:
   Triangle(const Triangle& rhs);

   /**
    * Make sure to specify the edges in the CLOCKWISE order,
    * otherwise the collision tests will fail
    */
   Triangle(const D3DXVECTOR3& pt1,
            const D3DXVECTOR3& pt2,
            const D3DXVECTOR3& pt3);

   const D3DXVECTOR3& vertex(unsigned int idx) const
   {
      ASSERT(idx <= 2, "Vertex index should be <= 2");
      return v[idx];
   }

   const D3DXVECTOR3& edge(unsigned int idx) const
   {
      ASSERT(idx <= 2, "Edge index should be <= 2");

      return e[idx];
   }

   const D3DXVECTOR3& edgeNormal(unsigned int idx) const
   {
      ASSERT(idx <= 2, "Edge normal index should be <= 2");

      return en[idx];
   }

};

///////////////////////////////////////////////////////////////////////////////
