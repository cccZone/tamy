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
#include "core-Renderer\RenderingParams.h"
// ----------------------------------------------------------------------------
// --> BasicRenderCommands
// ----------------------------------------------------------------------------
#include "core-Renderer\BasicRenderCommands.h"
// ----------------------------------------------------------------------------
// --> Camera
// ----------------------------------------------------------------------------
#include "core-Renderer\Camera.h"
// ----------------------------------------------------------------------------
// --> Tools
// ----------------------------------------------------------------------------
#include "core-Renderer\ShaderCompiler.h"
// ----------------------------------------------------------------------------
// --> RenderableSurfaces
// ----------------------------------------------------------------------------
#include "core-Renderer\ShaderTexture.h"
#include "core-Renderer\RenderTarget.h"
// ----------------------------------------------------------------------------
// --> RenderTree
// ----------------------------------------------------------------------------
#include "core-Renderer\RenderTree.h"
#include "core-Renderer\StatefulRenderTreeBuilder.h"
// ----------------------------------------------------------------------------
// --> Resources
// ----------------------------------------------------------------------------
#include "core-Renderer\RenderResource.h"
#include "core-Renderer\RenderResourceStorage.h"

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
#include "core-Renderer\GeometryEntity.h"
// ----------------------------------------------------------------------------
// -->Core
// ----------------------------------------------------------------------------
#include "core-Renderer\Face.h"
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\VertexDescriptors.h"
// ----------------------------------------------------------------------------
// -->GeometryShader
// ----------------------------------------------------------------------------
#include "core-Renderer\GeometryShaderNode.h"
#include "core-Renderer\GeometryShader.h"
#include "core-Renderer\GeometrySockets.h"
// ----------------------------------------------------------------------------
// ----> Nodes
// ----------------------------------------------------------------------------
#include "core-Renderer\GNCamera.h"
#include "core-Renderer\GNFloat.h"
#include "core-Renderer\GNVertexShader.h"
#include "core-Renderer\GNSpatialEntity.h"
#include "core-Renderer\GNTexture.h"
#include "core-Renderer\GNVec4.h"
#include "core-Renderer\GNBool.h"
#include "core-Renderer\GNTimer.h"
// ----------------------------------------------------------------------------
// -->Resources
// ----------------------------------------------------------------------------
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\TriangleMesh.h"
#include "core-Renderer\LineSegments.h"
#include "core-Renderer\Skeleton.h"
#include "core-Renderer\VertexShader.h"
#include "core-Renderer\FragmentShader.h"

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
#include "core-Renderer\MaterialEntity.h"
// ----------------------------------------------------------------------------
// --> MaterialGraph
// ----------------------------------------------------------------------------
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialNode.h"
#include "core-Renderer\MaterialSockets.h"
// ----------------------------------------------------------------------------
// ----> Nodes
// ----------------------------------------------------------------------------
#include "core-Renderer\MNCamera.h"
#include "core-Renderer\MNFloat.h"
#include "core-Renderer\MNPixelShader.h"
#include "core-Renderer\MNSpatialEntity.h"
#include "core-Renderer\MNTexture.h"
#include "core-Renderer\MNVec4.h"
#include "core-Renderer\MNBool.h"
#include "core-Renderer\MNInstanceTexture.h"
#include "core-Renderer\MNSurfaceProperties.h"
#include "core-Renderer\MNTimer.h"
// ----------------------------------------------------------------------------
// --> Resources
// ----------------------------------------------------------------------------
#include "core-Renderer\SurfaceProperties.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\PixelShader.h"


// ----------------------------------------------------------------------------
// Mechanisms
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
#include "core-Renderer/RenderingPipelineSockets.h"
// ----------------------------------------------------------------------------
// ---->Nodes
// ----------------------------------------------------------------------------
#include "core-Renderer/RPAdapterNode.h"
#include "core-Renderer/RPSceneRenderNode.h"
#include "core-Renderer/RPStartNode.h"
#include "core-Renderer/RPPostProcessNode.h"
#include "core-Renderer/RPCameraNode.h"
#include "core-Renderer/RPFloats2Vec4Node.h"
#include "core-Renderer/RPTextureNode.h"
#include "core-Renderer/RPFloatNode.h"
#include "core-Renderer/RPVec4Node.h"
// ----------------------------------------------------------------------------
// ---->Tools
// ----------------------------------------------------------------------------
#include "core-Renderer\PixelShaderNodeOperator.h"
#include "core-Renderer\PixelShaderConstant.h"
#include "core-Renderer\RenderTargetDescriptor.h"
// ----------------------------------------------------------------------------
// ----> Views
// ----------------------------------------------------------------------------
#include "core-Renderer\RenderingView.h"
#include "core-Renderer\RenderState.h"
#include "core-Renderer\SceneRenderTreeBuilder.h"

// ----------------------------------------------------------------------------
// Lighting
// ----------------------------------------------------------------------------
#include "core-Renderer\Light.h"
// ----------------------------------------------------------------------------
// --> Entities
// ----------------------------------------------------------------------------
#include "core-Renderer\DirectionalLight.h"
#include "core-Renderer\PointLight.h"
// ----------------------------------------------------------------------------
// --> PipelineNodes
// ----------------------------------------------------------------------------
#include "core-Renderer\RPDeferredLightingNode.h"
#include "core-Renderer\RPSoftShadowsNode.h"

// ----------------------------------------------------------------------------
// Tools
// ----------------------------------------------------------------------------
#include "core-Renderer\BasicRenderingEntitiesFactory.h"
