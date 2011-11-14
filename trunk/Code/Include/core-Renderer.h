#pragma once

/// @file   core-Renderer.h
/// @brief  aggregate include file for the 'core-Renderer' project

// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------
#include "core-Renderer\Defines.h"

// ----------------------------------------------------------------------------
// Core
// ----------------------------------------------------------------------------
#include "core-Renderer\RenderingMechanism.h"
#include "core-Renderer\CompositeRenderingMechanism.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderCommand.h"
#include "core-Renderer\ShaderParam.h"

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
#include "core-Renderer\FullscreenQuad.h"
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
// Mechanisms
// -->DebugDraw
// ----------------------------------------------------------------------------
#include "core-Renderer\DebugDrawCommands.h"
// ----------------------------------------------------------------------------
// -->OverlayRenderer
// ---->Elements
// ----------------------------------------------------------------------------
#include "core-Renderer\Font.h"
#include "core-Renderer\TextField.h"
// ----------------------------------------------------------------------------
// ----> Mechanism
// ----------------------------------------------------------------------------
#include "core-Renderer\Overlay.h"
#include "core-Renderer\OverlayRenderingMechanism.h"

// ----------------------------------------------------------------------------
// -->RenderingPipeline
// ----------------------------------------------------------------------------
#include "core-Renderer/RenderingPipeline.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderTargetDescriptor.h"
#include "core-Renderer/RenderingPipelineSockets.h"
// ----------------------------------------------------------------------------
// ---->Nodes\Misc
// ----------------------------------------------------------------------------
#include "core-Renderer/RPAdapterNode.h"
#include "core-Renderer/RPSceneRenderNode.h"
#include "core-Renderer/RPStartNode.h"
#include "core-Renderer/RPPostProcessNode.h"
#include "core-Renderer/RPDebugRendererNode.h"
#include "core-Renderer/RPDepthNormalsNode.h"
// ----------------------------------------------------------------------------
// ---->Nodes\PostProcess
// ----------------------------------------------------------------------------
#include "core-Renderer/RPBlurNode.h"
#include "core-Renderer/RPBrightnessNode.h"
#include "core-Renderer/RPDownsampleNode.h"
#include "core-Renderer/RPHDRNode.h"
#include "core-Renderer/RPLuminanceNode.h"
// ----------------------------------------------------------------------------
// ---->Sockets
// ----------------------------------------------------------------------------
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/FloatSockets.h"
#include "core-Renderer/VoidSockets.h"
// ----------------------------------------------------------------------------
// -->RenderingPipeline
// ----------------------------------------------------------------------------
#include "core-Renderer/DebugDrawCommands.h"

// ----------------------------------------------------------------------------
// Scene
// ----------------------------------------------------------------------------
#include "core-Renderer\RenderTarget.h"

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
