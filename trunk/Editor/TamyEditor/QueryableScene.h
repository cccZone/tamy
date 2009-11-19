#pragma once

/// @file   TamyEditor\QueryableScene.h
/// @brief  definition of a tool that allows to query the scene

#include "core\SpatialStorage.h"
#include "core-Scene\SpatiallyQueryable.h"


///////////////////////////////////////////////////////////////////////////////

typedef SpatialStorage<SpatiallyQueryable> QueryableScene;

///////////////////////////////////////////////////////////////////////////////
