#pragma once

/// @file      core\PointVolume.h
/// @brief     a bounding volume with size of a point

#include "core\BoundingVolume.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * The structure describes a point volume.
 */
struct PointVolume : public BoundingVolume
{
   D3DXVECTOR3 point;

   /**
    * Constructor.
    *
    * @param point
    */
   PointVolume( const D3DXVECTOR3& point );

   // -------------------------------------------------------------------------
   // BoundingVolume implementation
   // -------------------------------------------------------------------------
   BoundingVolume* clone() const;
   void transform( const D3DXMATRIX& mtx, BoundingVolume& transformedVolume ) const;
   float distanceToPlane(const D3DXPLANE& plane) const;
   bool testCollision(const PointVolume& point) const;
   bool testCollision(const AABoundingBox& rhs) const;
   bool testCollision(const BoundingSphere& rhs) const;
   bool testCollision(const Frustum& rhs) const;
   bool testCollision(const Ray& rhs) const;
   bool testCollision(const Triangle& rhs) const;
   bool testCollision(const BoundingVolume& rhs) const {return rhs.testCollision(*this);}
};

///////////////////////////////////////////////////////////////////////////////
