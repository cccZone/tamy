#pragma once

/// @file   core.h
/// @brief  aggregate include file for the 'core' project


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
#include "core\Graph.h"
#include "core\Point.h"
#include "core\ResourceStorage.h"
#include "core\Stack.h"

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
// Misc
// ----------------------------------------------------------------------------
#include "core\GenericFactory.h"
#include "core\GraphBFS.h"
#include "core\GraphBuilder.h"
#include "core\GraphBuilderFactory.h"
#include "core\GraphDijkstra.h"
#include "core\Observer.h"
#include "core\Subject.h"
#include "core\Assert.h"
#include "core\Color.h"
#include "core\IntervalOperation.h"
#include "core\MatrixInterpolator.h"
#include "core\UniqueObject.h"

// ----------------------------------------------------------------------------
// ComponentsSystem
// ----------------------------------------------------------------------------
#include "core\Component.h"
#include "core\ComponentsManager.h"
// ----------------------------------------------------------------------------
// -->SingletonsManager
// ----------------------------------------------------------------------------
#include "core\SingletonsManager.h"

// ----------------------------------------------------------------------------
// Serialization
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
// WindowingSystem
// ----------------------------------------------------------------------------
#include "core\WindowBuilder.h"
#include "core\WindowMessagesProcessor.h"
