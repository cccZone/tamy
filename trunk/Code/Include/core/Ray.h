#pragma once

#include "core\BoundingVolume.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * The structure describes a ray.
 */
struct Ray : public BoundingVolume
{
   D3DXVECTOR3 origin;

   // direction should always be a normalized vector. If it's 
   // a problem (someone's changing it from outside),
   // restrict access with getters and setters
   D3DXVECTOR3 direction;

   /**
    * Default constructor.
    */
   Ray();

   /**
    * Constructor.
    *
    * @param origin
    * @param direction
    */
   Ray( const D3DXVECTOR3& _origin, const D3DXVECTOR3& _direction );

   /**
    * Returns a distance to the specified bounding volume.
    *
    * @param rhs
    */
   float getDistanceTo( const BoundingVolume& rhs ) const;

   // -------------------------------------------------------------------------
   // BoundingVolume implementation
   // -------------------------------------------------------------------------
   BoundingVolume* clone() const;
   void transform( const D3DXMATRIX& mtx, BoundingVolume& transformedVolume ) const;
   PlaneClassification classifyAgainsPlane( const D3DXPLANE& plane ) const;
   float distanceToPlane( const D3DXPLANE& plane ) const;
   bool testCollision( const PointVolume& point ) const;
   bool testCollision( const AABoundingBox& rhs ) const;
   bool testCollision( const BoundingSphere& rhs ) const;
   bool testCollision( const Frustum& rhs ) const;
   bool testCollision( const Ray& rhs ) const;
   bool testCollision( const Triangle& rhs ) const;
   bool testCollision( const BoundingVolume& rhs ) const { return rhs.testCollision( *this ); }
};

///////////////////////////////////////////////////////////////////////////////
