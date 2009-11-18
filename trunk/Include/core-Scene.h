#pragma once

/// @file   core-Scene.h
/// @brief  aggregate include file for the 'core-Scene' project


// ----------------------------------------------------------------------------
// Entities
// ----------------------------------------------------------------------------
#include "core-Scene\SpatiallyQueryable.h"
// ----------------------------------------------------------------------------
// -->Cameras
// ----------------------------------------------------------------------------
#include "core-Scene\CameraEntity.h"
// ----------------------------------------------------------------------------
// -->Lights
// ----------------------------------------------------------------------------
#include "core-Scene\DirectionalLightEntity.h"
#include "core-Scene\PointLightEntity.h"
#include "core-Scene\SpotLightEntity.h"
// ----------------------------------------------------------------------------
// -->Materials
// ----------------------------------------------------------------------------
#include "core-Scene\MaterialEntity.h"
// ----------------------------------------------------------------------------
// -->Movies
// ----------------------------------------------------------------------------
#include "core-Scene\AnimatedCutsceneEntity.h"
// ----------------------------------------------------------------------------
// -->Scenery
// ----------------------------------------------------------------------------
#include "core-Scene\SkyBoxEntity.h"
#include "core-Scene\StaticGeometryEntity.h"

// ----------------------------------------------------------------------------
// MVC
// ----------------------------------------------------------------------------
#include "core-Scene\Entity.h"
#include "core-Scene\EntityController.h"
#include "core-Scene\CompositeEntityController.h"
#include "core-Scene\Model.h"
#include "core-Scene\ModelView.h"

// ----------------------------------------------------------------------------
// Properties
// ----------------------------------------------------------------------------
#include "core-Scene\Property.h"
#include "core-Scene\Properties.h"
#include "core-Scene\PropertyEditor.h"
#include "core-Scene\PropertiesView.h"

// ----------------------------------------------------------------------------
// Reflection
// ----------------------------------------------------------------------------
#include "core-Scene\ClassesRegistry.h"
