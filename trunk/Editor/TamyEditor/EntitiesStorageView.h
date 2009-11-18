#pragma once

/// @file   TamyEditor\EntitiesStorageView.h
/// @brief  spatial storage for model entities

#include "core-Scene\ModelView.h"
#include "core\SpatialStorage.h"
#include "core\RegularOctree.h"


///////////////////////////////////////////////////////////////////////////////

class SpatiallyQueryable;

///////////////////////////////////////////////////////////////////////////////

/**
 * This view allows to extract queryable entities into a spatial storage,
 * thus giving the application way to query those entities 
 * in a spatially-oriented manner (i.e. bounding volume queries).
 */
class EntitiesStorageView : public ModelView, 
                            public SpatialStorage<SpatiallyQueryable>
{
private:
   RegularOctree<SpatiallyQueryable>* m_storage;

public:
   /**
    * Constructor.
    *
    * @param treeBB              bounding box of the entire tree
    * @param maxElemsPerSector   maximum elements per sector. If there are more 
    *                            elements in a given sector than this value,
    *                            the sector gets subdivided, unless the sector
    *                            is located at the depth == maxTreeDepth.
    * @param maxTreeDepth        maximum depth of the tree
    * @param initDepth           initial depth to which the tree should be 
    */
   EntitiesStorageView(const AABoundingBox& treeBB, 
                       unsigned int maxElemsPerSector = 64,
                       unsigned int maxTreeDepth = 5);
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

protected:
   void resetContents();
};

///////////////////////////////////////////////////////////////////////////////
