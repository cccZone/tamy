/// @file   core.h
/// @brief  aggregate include file for the 'core' project
#pragma once


// ----------------------------------------------------------------------------
// BoundingVolumes
// ----------------------------------------------------------------------------
#include "core\BoundingVolume.h"
#include "core\AABoundingBox.h"
#include "core\BoundingSpace.h"
#include "core\BoundingSphere.h"
#include "core\Frustum.h"
#include "core\PointVolume.h"
#include "core\Ray.h"
#include "core\Triangle.h"
#include "core\TriangleSplitter.h"

// ----------------------------------------------------------------------------
// DataStructures
// ----------------------------------------------------------------------------
#include "core\Array.h"
#include "core\ConstSizeArray.h"
#include "core\CellSpacePartition.h"
#include "core\Point.h"
#include "core\Stack.h"
#include "core\RuntimeData.h"
#include "core\RoundBuffer.h"
#include "core\MemoryPool.h"
// ----------------------------------------------------------------------------
// -->Graphs
// ----------------------------------------------------------------------------
#include "core\Graph.h"
#include "core\GraphAlgorithms.h"
#include "core\GraphBuilder.h"
#include "core\GraphBuilderNode.h"
#include "core\GraphBuilderSockets.h"

// ----------------------------------------------------------------------------
// Delegates
// ----------------------------------------------------------------------------
#include "core\Delegate.h"

// ----------------------------------------------------------------------------
// Filesystem
// ----------------------------------------------------------------------------
#include "core\File.h"
#include "core\Filesystem.h"
#include "core\StreamBuffer.h"
#include "core\FilePath.h"

// ----------------------------------------------------------------------------
// HierarchicalNodes
// ----------------------------------------------------------------------------
#include "core\Node.h"
#include "core\NodeObserver.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core\NodeAction.h"
#include "core\NodeActionsExecutor.h"

// ----------------------------------------------------------------------------
// Math
// ----------------------------------------------------------------------------
#include "core\Vector.h"
#include "core\EulerAngles.h"
#include "core\Matrix.h"

// ----------------------------------------------------------------------------
// Misc
// ----------------------------------------------------------------------------
#include "core\GenericFactory.h"
#include "core\Observer.h"
#include "core\Subject.h"
#include "core\Assert.h"
#include "core\Color.h"
#include "core\IntervalOperation.h"
#include "core\MatrixInterpolator.h"
#include "core\UniqueObject.h"
#include "core\StringUtils.h"
#include "core\IProgressObserver.h"
#include "core\Algorithms.h"
#include "core\Singleton.h"
#include "core\IDebugDraw.h"

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------
#include "core\Resource.h"
#include "core\ResourcesManager.h"
#include "core\ResourceManagerComponent.h"
#include "core\ResourceStorage.h"
#include "core\ResourceHandle.h"
#include "core\ResourceImporter.h"

// ----------------------------------------------------------------------------
// RTTI
// ----------------------------------------------------------------------------
#include "core\Object.h"
#include "core\Casts.h"
// ----------------------------------------------------------------------------
// -->Properties
// ----------------------------------------------------------------------------
#include "core\Property.h"
#include "core\Properties.h"
#include "core\PropertyEditor.h"
#include "core\PropertiesView.h"
// ----------------------------------------------------------------------------
// -->Classes
// ----------------------------------------------------------------------------
#include "core\ClassesRegistry.h"
#include "core\Class.h"
#include "core\Enum.h"
// ----------------------------------------------------------------------------
// -->Serialization
// ----------------------------------------------------------------------------
#include "core\Serializer.h"
#include "core\Serializable.h"
#include "core\FileSerializer.h"

// ----------------------------------------------------------------------------
// SpatialStorage
// ----------------------------------------------------------------------------
#include "core\SpatialStorage.h"
#include "core\BSPNodeTree.h"
#include "core\LinearStorage.h"
#include "core\Octree.h"
#include "core\DynamicOctree.h"
#include "core\RegularOctree.h"
#include "core\StaticGeometryOctree.h"
#include "core\AdaptingSpatialStorage.h"
#include "core\CompositeSpatialStorage.h"
#include "core\NarrowPhaseStorageFilter.h"
#include "core\SharedSpatialStorage.h"
#include "core\TNodesSpatialStorage.h"

// ----------------------------------------------------------------------------
// Streams
// ----------------------------------------------------------------------------
#include "core\dostream.h"

// ----------------------------------------------------------------------------
// Timer
// ----------------------------------------------------------------------------
#include "core\Timer.h"

// ----------------------------------------------------------------------------
// ComponentsSystem
// ----------------------------------------------------------------------------
#include "core\Component.h"
#include "core\ComponentsManager.h"
// ----------------------------------------------------------------------------
// -->SingletonsManager
// ----------------------------------------------------------------------------
#include "core\SingletonsManager.h"
