#include "core\Triangle.h"
#include "core\CollisionTests.h"
#include "core\Matrix.h"
#include "core\Plane.h"
#include "core\AABoundingBox.h"
#include "core\MathDataStorage.h"


///////////////////////////////////////////////////////////////////////////////

Triangle::Triangle( const Triangle& rhs )
{
   v[0] = rhs.v[0];
   v[1] = rhs.v[1];
   v[2] = rhs.v[2];

   e[0] = rhs.e[0];
   e[1] = rhs.e[1];
   e[2] = rhs.e[2];

   en[0] = rhs.en[0];
   en[1] = rhs.en[1];
   en[2] = rhs.en[2];
}

///////////////////////////////////////////////////////////////////////////////

Triangle::Triangle( const Vector& pt1, const Vector& pt2, const Vector& pt3 )
{
   initFromCoplanarPoints( pt1, pt2, pt3 );
}

///////////////////////////////////////////////////////////////////////////////

void Triangle::initFromCoplanarPoints( const Vector& pt1, const Vector& pt2, const Vector& pt3 )
{
   v[0] = pt1;
   v[1] = pt2;
   v[2] = pt3;
 
   // calculate the edges
   e[0].setSub( v[1], v[0] );
   e[1].setSub( v[2], v[1] );
   e[2].setSub( v[0], v[2] );

   // calculate the edge normals
   Vector tmpPerpVec;
   tmpPerpVec.setCross( e[1], e[0] ).preCross( e[0] );
   en[0].setNormalized( tmpPerpVec );

   tmpPerpVec.setCross( e[2], e[1] ).preCross( e[1] );
   en[1].setNormalized( tmpPerpVec );

   tmpPerpVec.setCross( e[0], e[2] ).preCross( e[2] );
   en[2].setNormalized( tmpPerpVec );
}

///////////////////////////////////////////////////////////////////////////////

BoundingVolume* Triangle::clone() const
{
   return new Triangle( *this );
}

///////////////////////////////////////////////////////////////////////////////

void Triangle::transform( const Matrix& mtx, BoundingVolume& transformedVolume ) const
{
   // verify that the volume is a Triangle
   ASSERT( dynamic_cast< Triangle* >( &transformedVolume ) != NULL );
   Triangle& transformedTriangle = static_cast< Triangle& >( transformedVolume );

   Vector newV[3];
   mtx.transform( v[0], newV[0] );
   mtx.transform( v[1], newV[1] );
   mtx.transform( v[2], newV[2] );

   transformedTriangle.initFromCoplanarPoints( newV[0], newV[1], newV[2] );
}

///////////////////////////////////////////////////////////////////////////////

void Triangle::calculateBoundingBox( AABoundingBox& outBoundingBox ) const
{
   outBoundingBox.include( v[0] );
   outBoundingBox.include( v[1] );
   outBoundingBox.include( v[2] );
}

///////////////////////////////////////////////////////////////////////////////

void Triangle::splitEdge( const FastFloat& percentage, unsigned int startVtxIdx, unsigned int endVtxIdx, Vector& outEdge ) const
{
   const Vector& v1 = v[startVtxIdx];
   Vector edge;
   edge.setSub( v[endVtxIdx], v1 );

   outEdge.setMulAdd( edge, percentage.getFloat(), v1 );
}

///////////////////////////////////////////////////////////////////////////////

PlaneClassification Triangle::classifyAgainsPlane( const Plane& plane ) const
{
    const FastFloat dist1 = plane.dotCoord( v[0] );
    const FastFloat dist2 = plane.dotCoord( v[1] );
    const FastFloat dist3 = plane.dotCoord( v[2] );

    if ( ( dist1 < Float_0 ) && ( dist2 < Float_0 ) && ( dist3 < Float_0 ) )
    {
        return PPC_BACK;
    }
    else if ( ( dist1 > Float_0 ) && ( dist2 > Float_0 ) && ( dist3 > Float_0 ) )
    {
        return PPC_FRONT;
    }
    else if ( ( dist1 == Float_0 ) && ( dist2 == Float_0 ) && ( dist3 == Float_0 ) )
    {
        return PPC_COPLANAR;
    }
    else
    {
        return PPC_SPANNING;
    }
}

///////////////////////////////////////////////////////////////////////////////

float Triangle::distanceToPlane( const Plane& plane ) const
{
   FastFloat posEps, negEps;
   posEps = Float_1e_4;
   negEps.setNeg( Float_1e_4 );

   int frontCount = 0;
   int backCount = 0;
   for (unsigned int i = 0; i < 3; ++i)
   {
      Vector vtx = v[i];
      vtx.w = 1;

      const FastFloat dist = plane.dotCoord( vtx );
      if ( dist > posEps )
      {
         ++frontCount;
      }
      else if ( dist < negEps )
      {
         ++backCount;
      }
   }

   if ( frontCount == 3 ) 
   {
      return 1;
   }
   else if ( backCount == 3 ) 
   {
      return -1;
   }
   else 
   {
      return 0;
   }
}

///////////////////////////////////////////////////////////////////////////////

void Triangle::split(const Plane& splitPlane, Array< Triangle* >& frontSplit, Array< Triangle* >& backSplit)
{
   m_splitter.split( *this, splitPlane, frontSplit, backSplit );
}

///////////////////////////////////////////////////////////////////////////////

bool Triangle::testCollision( const PointVolume& point ) const
{
   ASSERT_MSG(false, "Triangle::testCollision(const PointVolume&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Triangle::testCollision( const AABoundingBox& rhs ) const
{
   ASSERT_MSG(false, "Triangle::testCollision(const AABoundingBox&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Triangle::testCollision( const BoundingSphere& rhs ) const
{
   ASSERT_MSG(false, "Triangle::testCollision(const BoundingSphere&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Triangle::testCollision( const Frustum& rhs ) const
{
   ASSERT_MSG(false, "Triangle::testCollision(const Frustum&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Triangle::testCollision( const Ray& rhs ) const
{
   return ::testCollision(rhs, *this);
}

///////////////////////////////////////////////////////////////////////////////

bool Triangle::testCollision( const Triangle& rhs ) const
{
   ASSERT_MSG(false, "Triangle::testCollision(const Triangle&) - Method not implemented");
   return false;
}

///////////////////////////////////////////////////////////////////////////////
