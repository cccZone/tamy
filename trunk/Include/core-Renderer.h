#pragma once

/// @file   core-Renderer.h
/// @brief  aggregate include file for the 'core-Renderer' project


// ----------------------------------------------------------------------------
// Core
// ----------------------------------------------------------------------------
#include "core-Renderer\Renderable.h"
#include "core-Renderer\RenderablesStorage.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RendererImpl.h"
#include "core-Renderer\RenderingAction.h"
#include "core-Renderer\RenderingActionsComposite.h"
#include "core-Renderer\RenderingMechanism.h"
#include "core-Renderer\RenderingTargetsCleaner.h"
#include "core-Renderer\RenderingTargetsPolicy.h"
#include "core-Renderer\TargetsCleaningMechanism.h"
#include "core-Renderer\CompositeRenderingMechanism.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"
#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\TextureRenderingTarget.h"
#include "core-Renderer\CyclicRenderingTarget.h"

// ----------------------------------------------------------------------------
// Effects
// ----------------------------------------------------------------------------
#include "core-Renderer\GraphicalEffect.h"
#include "core-Renderer\RenderingEffectRenderer.h"

// ----------------------------------------------------------------------------
// Factories
// ----------------------------------------------------------------------------
#include "core-Renderer\GraphicalEntitiesFactory.h"

// ----------------------------------------------------------------------------
// Geometry
// ----------------------------------------------------------------------------
#include "core-Renderer\GeometryRenderingMechanism.h"
// ----------------------------------------------------------------------------
// --> Backgrounds
// ----------------------------------------------------------------------------
#include "core-Renderer\SkyBoxSide.h"
#include "core-Renderer\SkyBoxStorage.h"
// ----------------------------------------------------------------------------
// --> Meshes
// ----------------------------------------------------------------------------
#include "core-Renderer\RenderableNode.h"
// ----------------------------------------------------------------------------
// -->--> GraphicalData
// ----------------------------------------------------------------------------
#include "core-Renderer\Face.h"
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\MeshTriangle.h"
// ----------------------------------------------------------------------------
// -->--> GraphicalEntities
// ----------------------------------------------------------------------------
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\LeafGraphicalEntity.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\SkinnedGraphicalEntity.h"
#include "core-Renderer\CompositeGraphicalEntity.h"
#include "core-Renderer\SkinBoneDefinition.h"
// ----------------------------------------------------------------------------
// -->--> GraphicalNodes
// ----------------------------------------------------------------------------
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\SkinnedGraphicalNode.h"
// ----------------------------------------------------------------------------
// -->--> StaticGeometry
// ----------------------------------------------------------------------------
#include "core-Renderer\StaticGeometryRenderable.h"
// ----------------------------------------------------------------------------
// --> Misc
// ----------------------------------------------------------------------------
#include "core-Renderer\Camera.h"
#include "core-Renderer\ProjectionCalculator.h"
#include "core-Renderer\ProjCalc2D.h"
#include "core-Renderer\ProjCalc3D.h"
#include "core-Renderer\PostProcessEffectRenderable.h"
#include "core-Renderer\Skeleton.h"
// ----------------------------------------------------------------------------
// --> Particles
// ----------------------------------------------------------------------------
#include "core-Renderer\Particle.h"
#include "core-Renderer\ParticleAnimator.h"
#include "core-Renderer\ParticleInitializer.h"
#include "core-Renderer\ParticleSystem.h"
// ----------------------------------------------------------------------------
// -->--> Animators
// ----------------------------------------------------------------------------
#include "core-Renderer\ParticleFader.h"
// ----------------------------------------------------------------------------
// -->--> Initializers
// ----------------------------------------------------------------------------
#include "core-Renderer\CircularParticleInitializer.h"
#include "core-Renderer\PlanarParticleInitializer.h"
#include "core-Renderer\PointParticleInitializer.h"

// ----------------------------------------------------------------------------
// Init
// ----------------------------------------------------------------------------
#include "core-Renderer\DeviceFilter.h"
#include "core-Renderer\DeviceFilterElem.h"
#include "core-Renderer\DevicesSorter.h"
#include "core-Renderer\RenderDevicesEnumerator.h"
#include "core-Renderer\RenderingDevice.h"

// ----------------------------------------------------------------------------
// Material
// ----------------------------------------------------------------------------
#include "core-Renderer\CoordinatesOperation.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\MaterialStage.h"
// ----------------------------------------------------------------------------
// --> Editors
// ----------------------------------------------------------------------------
#include "core-Renderer\MaterialReplacer.h"
// ----------------------------------------------------------------------------
// --> RenderingActions
// ----------------------------------------------------------------------------
#include "core-Renderer\RenderMaterial.h"
// ----------------------------------------------------------------------------
// --> Textures
// ----------------------------------------------------------------------------
#include "core-Renderer\Texture.h"
#include "core-Renderer\ChangableTexture.h"
#include "core-Renderer\ManagedTexture.h"

// ----------------------------------------------------------------------------
// Overlay
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
// RenderableStorages
// -->PostProcessEffects
// ----------------------------------------------------------------------------
#include "core-Renderer\PostProcessSceneStorage.h"
// ----------------------------------------------------------------------------
// --> SortingStorage
// ----------------------------------------------------------------------------
#include "core-Renderer\SortingRenderablesStorage.h"

// ----------------------------------------------------------------------------
// SceneLighting
// ----------------------------------------------------------------------------
#include "core-Renderer\Light.h"
// ----------------------------------------------------------------------------
// --> SortingStorage
// ----------------------------------------------------------------------------
#include "core-Renderer\RenderLights.h"
#include "core-Renderer\LightsStorage.h"
