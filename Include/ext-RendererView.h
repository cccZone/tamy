#pragma once

/// @file  ext-RendererView\ext-RendererView.h
/// @brief  aggregate include file for the 'ext-RendererView' project


// ----------------------------------------------------------------------------
// RendererView
// ----------------------------------------------------------------------------
#include "ext-RendererView\RendererView.h"
#include "ext-RendererView\GraphicalRepresentation.h"

// ----------------------------------------------------------------------------
// Representations
// -->Cameras
// ----------------------------------------------------------------------------
#include "ext-RendererView\Camera.h"
// ----------------------------------------------------------------------------
// -->Lights
// ----------------------------------------------------------------------------
#include "ext-RendererView\DirectionalLight.h"
#include "ext-RendererView\SpotLight.h"
#include "ext-RendererView\PointLight.h"
#include "ext-RendererView\LightsScene.h"
// ----------------------------------------------------------------------------
// -->Materials
// ----------------------------------------------------------------------------
#include "ext-RendererView\Material.h"
// ----------------------------------------------------------------------------
// -->Movies
// ----------------------------------------------------------------------------
#include "ext-RendererView\AnimatedCutscene.h"
// ----------------------------------------------------------------------------
// -->Scenery
// ----------------------------------------------------------------------------
#include "ext-RendererView\SkyBox.h"
#include "ext-RendererView\StaticGeometry.h"
