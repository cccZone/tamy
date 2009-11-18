#pragma once

/// @file   core-ResourceManagement.h
/// @brief  aggregate include file for the 'core-ResourceManagement' project


// ----------------------------------------------------------------------------
// Builder
// ----------------------------------------------------------------------------
#include "core-ResourceManagement\GeometryBuilder.h"

// ----------------------------------------------------------------------------
// Loaders
// -->GraphicalEntitiesLoader
// ----------------------------------------------------------------------------
#include "core-ResourceManagement\MeshDefinition.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "core-ResourceManagement\GraphicalDataSource.h"
#include "core-ResourceManagement\FileGraphicalDataSource.h"
#include "core-ResourceManagement\CompositeGraphicalDataSource.h"
// ----------------------------------------------------------------------------
// -->IWF
// ----------------------------------------------------------------------------
#include "core-ResourceManagement\IWFScene.h"
#include "core-ResourceManagement\IWFLoader.h"
// ----------------------------------------------------------------------------
// -->StaticGeometryLoader
// ----------------------------------------------------------------------------
#include "core-ResourceManagement\StaticGeometryLoader.h"
#include "core-ResourceManagement\StaticGeometry.h"
#include "core-ResourceManagement\MeshHierarchyFlattener.h"

// ----------------------------------------------------------------------------
// SingletonsManager
// ----------------------------------------------------------------------------
#include "core-ResourceManagement\Managable.h"
#include "core-ResourceManagement\SingletonsManager.h"

