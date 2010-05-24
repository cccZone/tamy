#pragma once

/// @file   core-Renderer.h
/// @brief  aggregate include file for the 'core-Renderer' project


// ----------------------------------------------------------------------------
// Core
// ----------------------------------------------------------------------------
#include "core-Renderer\RenderingMechanism.h"
#include "core-Renderer\CompositeRenderingMechanism.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RendererObject.h"
#include "core-Renderer\RendererObjectImpl.h"

// ----------------------------------------------------------------------------
// Components
// ----------------------------------------------------------------------------
#include "core-Renderer\CameraComponent.h"
#include "core-Renderer\RendererComponent.h"

// ----------------------------------------------------------------------------
// Effects
// ----------------------------------------------------------------------------
#include "core-Renderer\SingleTextureEffect.h"

// ----------------------------------------------------------------------------
// Geometry
// ----------------------------------------------------------------------------
#include "core-Renderer\Face.h"
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\TriangleMesh.h"
#include "core-Renderer\LineSegments.h"

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------
#include "core-Renderer\DeviceFilter.h"
#include "core-Renderer\DeviceFilterElem.h"
#include "core-Renderer\DevicesSorter.h"
#include "core-Renderer\RenderDevicesEnumerator.h"
#include "core-Renderer\RenderingDevice.h"
// ----------------------------------------------------------------------------
// -->Simple
// ----------------------------------------------------------------------------
#include "core-Renderer\SimpleTamyConfigurator.h"

// ----------------------------------------------------------------------------
// Materials
// ----------------------------------------------------------------------------
#include "core-Renderer\Material.h"
#include "core-Renderer\Texture.h"

// ----------------------------------------------------------------------------
// OverlayRenderer
// -->Elements
// ----------------------------------------------------------------------------
#include "core-Renderer\Font.h"
#include "core-Renderer\TextField.h"
// ----------------------------------------------------------------------------
// --> Mechanism
// ----------------------------------------------------------------------------
#include "core-Renderer\Overlay.h"
#include "core-Renderer\OverlayRenderingMechanism.h"

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------
#include "core-Renderer\Shader.h"
#include "core-Renderer\GeometryResource.h"

// ----------------------------------------------------------------------------
// Scene
// ----------------------------------------------------------------------------
#include "core-Renderer\Scene3DRM.h"
// ----------------------------------------------------------------------------
// --> Entities
// ----------------------------------------------------------------------------
#include "core-Renderer\SpatialEntity.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\RenderableJoint.h"
#include "core-Renderer\Effect.h"
#include "core-Renderer\ShaderEffect.h"
#include "core-Renderer\EffectAttribute.h"
#include "core-Renderer\Geometry.h"
// ----------------------------------------------------------------------------
// --> Views
// ----------------------------------------------------------------------------
#include "core-Renderer\UniqueObjectAttribute.h"
#include "core-Renderer\TransparencyAttribute.h"

// ----------------------------------------------------------------------------
// Tools
// ----------------------------------------------------------------------------
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\ProjectionCalculator.h"
#include "core-Renderer\Skeleton.h"
// ----------------------------------------------------------------------------
// --> ProjectionCalculators
// ----------------------------------------------------------------------------
#include "core-Renderer\ProjCalc2D.h"
#include "core-Renderer\ProjCalc3D.h"
