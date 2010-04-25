#ifndef _QUERYABLE_GHOST_H
#define _QUERYABLE_GHOST_H

/// @file   TamyEditor\QueryableGhost.h
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
class QueryableGhost : public SceneQueriesModelRepresentation,
                       public SpatiallyQueryable
{
private:
   T& m_entity;
   Array<Triangle*> m_noGeometry;
   BoundingSphere m_vol;

public:
   /**
    * Constructor.
    *
    */
   QueryableGhost(T& entity);
   ~QueryableGhost();

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

#include "QueryableGhost.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _QUERYABLE_GHOST_H
