#pragma once

/// @file   TamyEditor\QueryableScene.h
/// @brief  definition of a tool that allows to query the scene

#include "core\SpatialStorage.h"


///////////////////////////////////////////////////////////////////////////////

class SpatiallyQueryable;

///////////////////////////////////////////////////////////////////////////////

class QueryableScene : public SpatialStorage<SpatiallyQueryable>
{
public:
   virtual ~QueryableScene() {}

   virtual void insert(SpatiallyQueryable& entity) = 0;

   virtual void remove(SpatiallyQueryable& entity) = 0;
};

///////////////////////////////////////////////////////////////////////////////
