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
// Effects
// -->Entities
// ----------------------------------------------------------------------------
#include "core-Renderer\SingleTextureEffect.h"
// ----------------------------------------------------------------------------
// -->Resources
// ----------------------------------------------------------------------------
#include "core-Renderer\EffectShader.h"

// ----------------------------------------------------------------------------
// Geometry
// -->Entities
// ----------------------------------------------------------------------------
#include "core-Renderer\StaticGeometry.h"
#include "core-Renderer\SkinnedGeometry.h"
// ----------------------------------------------------------------------------
// -->Primitives
// ----------------------------------------------------------------------------
#include "core-Renderer\Face.h"
#include "core-Renderer\LitVertex.h"
// ----------------------------------------------------------------------------
// -->Resources
// ----------------------------------------------------------------------------
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\TriangleMesh.h"
#include "core-Renderer\LineSegments.h"
#include "core-Renderer\Skeleton.h"
#include "core-Renderer\VertexShader.h"

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
// --> Entities
// ----------------------------------------------------------------------------
#include "core-Renderer\SingleTextureMaterial.h"
// ----------------------------------------------------------------------------
// --> Resources
// ----------------------------------------------------------------------------
#include "core-Renderer\Material.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\PixelShader.h"

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
// Renderers
// ----------------------------------------------------------------------------
#include "core-Renderer\HDRPipeline.h"

// ----------------------------------------------------------------------------
// Scene
// ----------------------------------------------------------------------------
#include "core-Renderer\RenderingPass.h"
#include "core-Renderer\SceneRenderingPass.h"
#include "core-Renderer\VisibilityPass.h"
#include "core-Renderer\AttributeSorter.h"
#include "core-Renderer\EffectAttribute.h"
#include "core-Renderer\RenderTarget.h"
// ----------------------------------------------------------------------------
// --> Attributes
// ----------------------------------------------------------------------------
#include "core-Renderer\TransparencyAttribute.h"
#include "core-Renderer\UniqueObjectAttribute.h"
// ----------------------------------------------------------------------------
// --> Entities
// ----------------------------------------------------------------------------
#include "core-Renderer\SpatialEntity.h"
// ----------------------------------------------------------------------------
// --> Views
// ----------------------------------------------------------------------------
#include "core-Renderer\UniqueObjectAttribute.h"
#include "core-Renderer\TransparencyAttribute.h"
#include "core-Renderer\Renderable.h"

// ----------------------------------------------------------------------------
// Tools
// ----------------------------------------------------------------------------
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\ProjectionCalculator.h"
// ----------------------------------------------------------------------------
// --> ProjectionCalculators
// ----------------------------------------------------------------------------
#include "core-Renderer\ProjCalc2D.h"
#include "core-Renderer\ProjCalc3D.h"
