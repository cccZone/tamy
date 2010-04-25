#ifndef _QUERYABLE_SPATIAL_H
#define _QUERYABLE_SPATIAL_H

/// @file   TamyEditor\QueryableSpatial.h
/// @brief  queryable representation of an entity with no bounds

#include "SceneQueriesModelRepresentation.h"
#include "core-MVC\Entity.h"
#include "SpatiallyQueryable.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

class SpatiallyQueryable;

///////////////////////////////////////////////////////////////////////////////

/**
 * This representation will allow to query entities that have 
 * a bounding volume.
 */
template <typename T>
class QueryableSpatial : public SceneQueriesModelRepresentation,
                         public SpatiallyQueryable
{
private:
   T& m_entity;
   const BoundingVolume& m_localVol;
   BoundingVolume* m_globalVol;
   Array<Triangle*> m_noTriangles;

public:
   /**
    * Constructor.
    *
    */
   QueryableSpatial(T& entity);
   ~QueryableSpatial();

   // -------------------------------------------------------------------------
   // SceneQueriesModelRepresentation implementation
   // -------------------------------------------------------------------------
   void initialize(SceneQueries& parent);

   void deinitialize(SceneQueries& parent);

   // -------------------------------------------------------------------------
   // SpatiallyQueryable implementation
   // -------------------------------------------------------------------------
   const BoundingVolume& getBoundingVolume();

   const Array<Triangle*>& getBoundingGeometry();

   const D3DXMATRIX& getGlobalMtx();

   Entity& getEntity();
};

///////////////////////////////////////////////////////////////////////////////

#include "QueryableSpatial.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _QUERYABLE_SPATIAL_H
