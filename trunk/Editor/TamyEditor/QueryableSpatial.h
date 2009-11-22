#ifndef _QUERYABLE_SPATIAL_H
#define _QUERYABLE_SPATIAL_H

/// @file   TamyEditor\QueryableSpatial.h
/// @brief  queryable representation of an entity with no bounds

#include "SceneQueriesModelRepresentation.h"
#include "core-Scene\Entity.h"
#include "SpatiallyQueryable.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

class SpatiallyQueryable;

///////////////////////////////////////////////////////////////////////////////

/**
 * This representation will allow to query entities that don't have 
 * a volume (like cameras, lights etc.)
 */
template <typename T>
class QueryableSpatial : public SceneQueriesModelRepresentation,
                         public SpatiallyQueryable
{
private:
   T& m_entity;

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
