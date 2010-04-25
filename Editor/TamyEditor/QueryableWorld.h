#pragma once

/// @file   TamyEditor\EntitiesStorageView.h
/// @brief  representation of a queryable world entity

#include "SceneQueriesModelRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

class SpatiallyQueryable;
struct WorldEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * This view allows to extract queryable entities into a spatial storage,
 * thus giving the application way to query those entities 
 * in a spatially-oriented manner (i.e. bounding volume queries).
 */
class QueryableWorld : public SceneQueriesModelRepresentation
{
private:
   WorldEntity& m_entity;

public:
   /**
    * Constructor.
    */
   QueryableWorld(WorldEntity& entity);
   // -------------------------------------------------------------------------
   // SceneQueriesModelRepresentation implementation
   // -------------------------------------------------------------------------
   void initialize(SceneQueries& parent);

   void deinitialize(SceneQueries& parent);
};

///////////////////////////////////////////////////////////////////////////////
