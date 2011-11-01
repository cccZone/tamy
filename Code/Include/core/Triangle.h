#pragma once

/// @file   core\Triangle.h
/// @brief  a triangle representation

#include "core\BoundingVolume.h"
#include <d3dx9.h>
#include "core\Assert.h"
#include "core\TriangleSplitter.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This structure represents a triangle with some basic operations
 * we can perform on it.
 */
struct Triangle : public BoundingVolume,
                  public SplittableTriangle<D3DXVECTOR3>
{
private:
   D3DXVECTOR3 v[3];
   D3DXVECTOR3 e[3];
   D3DXVECTOR3 en[3];

   TriangleSplitter<D3DXVECTOR3, Triangle> m_splitter;

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
      ASSERT_MSG(idx <= 2, "Vertex index should be <= 2");
      return v[idx];
   }

   const D3DXVECTOR3& vertexPos(unsigned int idx) const
   {
      ASSERT_MSG(idx <= 2, "Vertex index should be <= 2");
      return v[idx];
   }

   const D3DXVECTOR3& edge(unsigned int idx) const
   {
      ASSERT_MSG(idx <= 2, "Edge index should be <= 2");

      return e[idx];
   }

   const D3DXVECTOR3& edgeNormal(unsigned int idx) const
   {
      ASSERT_MSG(idx <= 2, "Edge normal index should be <= 2");

      return en[idx];
   }

   D3DXVECTOR3 splitEdge(float percentage,
                         unsigned int startVtxIdx, 
                         unsigned int endVtxIdx) const;

   void split(const D3DXPLANE& splitPlane, Array<Triangle*>& frontSplit, Array<Triangle*>& backSplit);

   // -------------------------------------------------------------------------
   // BoundingVolume implementation
   // -------------------------------------------------------------------------
   BoundingVolume* clone() const;
   void transform( const D3DXMATRIX& mtx, BoundingVolume& transformedVolume ) const;
   float distanceToPlane(const D3DXPLANE& plane) const;
   PlaneClassification classifyAgainsPlane(const D3DXPLANE& plane) const;
   bool testCollision(const PointVolume& point) const;
   bool testCollision(const AABoundingBox& rhs) const;
   bool testCollision(const BoundingSphere& rhs) const;
   bool testCollision(const Frustum& rhs) const;
   bool testCollision(const Ray& rhs) const;
   bool testCollision(const Triangle& rhs) const;
   bool testCollision(const BoundingVolume& rhs) const { return rhs.testCollision(*this); }

protected:
   void initFromCoplanarPoints( const D3DXVECTOR3& pt1, const D3DXVECTOR3& pt2, const D3DXVECTOR3& pt3 );
};

///////////////////////////////////////////////////////////////////////////////
