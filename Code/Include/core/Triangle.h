/// @file   core\Triangle.h
/// @brief  a triangle representation
#pragma once

#include "core\BoundingVolume.h"
#include "core\Vector.h"
#include "core\Assert.h"
#include "core\TriangleSplitter.h"
#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

struct Plane;

///////////////////////////////////////////////////////////////////////////////

/**
 * This structure represents a triangle with some basic operations
 * we can perform on it.
 */
struct Triangle : public BoundingVolume, public SplittableTriangle< Vector >
{
   DECLARE_ALLOCATOR( Triangle, AM_ALIGNED_16 );

private:
   Vector v[3];
   Vector e[3];
   Vector en[3];

   TriangleSplitter< Vector, Triangle > m_splitter;

public:
   Triangle( const Triangle& rhs );

   /**
    * Make sure to specify the edges in the CLOCKWISE order,
    * otherwise the collision tests will fail
    */
   Triangle( const Vector& pt1, const Vector& pt2, const Vector& pt3 );

   const Vector& vertex( unsigned int idx ) const
   {
      ASSERT_MSG(idx <= 2, "Vertex index should be <= 2");
      return v[idx];
   }

   const Vector& vertexPos( unsigned int idx ) const
   {
      ASSERT_MSG(idx <= 2, "Vertex index should be <= 2");
      return v[idx];
   }

   const Vector& edge( unsigned int idx ) const
   {
      ASSERT_MSG(idx <= 2, "Edge index should be <= 2");

      return e[idx];
   }

   const Vector& edgeNormal( unsigned int idx ) const
   {
      ASSERT_MSG(idx <= 2, "Edge normal index should be <= 2");

      return en[idx];
   }

   // -------------------------------------------------------------------------
   // SplittableTriangle implementation
   // -------------------------------------------------------------------------
   void splitEdge( const FastFloat& percentage, unsigned int startVtxIdx, unsigned int endVtxIdx, Vector& outEdge ) const;

   void split( const Plane& splitPlane, Array< Triangle* >& frontSplit, Array< Triangle* >& backSplit );

   // -------------------------------------------------------------------------
   // BoundingVolume implementation
   // -------------------------------------------------------------------------
   BoundingVolume* clone() const;
   void transform( const Matrix& mtx, BoundingVolume& transformedVolume ) const;
   void calculateBoundingBox( AABoundingBox& outBoundingBox ) const;
   const FastFloat distanceToPlane( const Plane& plane ) const;
   PlaneClassification classifyAgainsPlane( const Plane& plane ) const;
   bool testCollision( const PointVolume& point ) const;
   bool testCollision( const AABoundingBox& rhs ) const;
   bool testCollision( const BoundingSphere& rhs ) const;
   bool testCollision( const Frustum& rhs ) const;
   bool testCollision( const Ray& rhs ) const;
   bool testCollision( const Triangle& rhs ) const;
   bool testCollision( const BoundingVolume& rhs ) const { return rhs.testCollision( *this ); }

protected:
   void initFromCoplanarPoints( const Vector& pt1, const Vector& pt2, const Vector& pt3 );
};

///////////////////////////////////////////////////////////////////////////////
