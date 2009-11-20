#pragma once

/// @file   TamyEditor\EntitiesStorageView.h
/// @brief  spatial storage for model entities

#include "SceneQueriesModelRepresentation.h"
#include "core-Scene\ModelView.h"
#include "core\SpatialStorage.h"
#include "core\RegularOctree.h"


///////////////////////////////////////////////////////////////////////////////

class SpatiallyQueryable;
struct WorldEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * This view allows to extract queryable entities into a spatial storage,
 * thus giving the application way to query those entities 
 * in a spatially-oriented manner (i.e. bounding volume queries).
 */
class EntitiesStorageView : public ModelView, 
                            public SpatialStorage<SpatiallyQueryable>,
                            public SceneQueriesModelRepresentation
{
private:
   WorldEntity& m_entity;
   RegularOctree<SpatiallyQueryable>* m_storage;

public:
   /**
    * Constructor.
    *
    */
   EntitiesStorageView(WorldEntity& entity);
   ~EntitiesStorageView();

   // -------------------------------------------------------------------------
   // SpatialStorage implementation
   // -------------------------------------------------------------------------
   void query(const BoundingVolume& boundingVol, 
              Array<SpatiallyQueryable*>& output) const;

   // -------------------------------------------------------------------------
   // ModelView implementation
   // -------------------------------------------------------------------------
   void onEntityAdded(Entity& entity);

   void onEntityRemoved(Entity& entity);

   // -------------------------------------------------------------------------
   // SceneQueriesModelRepresentation implementation
   // -------------------------------------------------------------------------
   void initialize(SceneQueries& parent);

   void deinitialize(SceneQueries& parent);

protected:
   void resetContents();
};

///////////////////////////////////////////////////////////////////////////////
