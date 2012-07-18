/// @file   core.h
/// @brief  aggregate include file for the 'core' project
#pragma once

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
#include "core\FilesystemSection.h"
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
#include "core\Math.h"

// ----------------------------------------------------------------------------
// Misc
// ----------------------------------------------------------------------------
#include "core\Color.h"
#include "core\GenericFactory.h"
#include "core\Observer.h"
#include "core\Subject.h"
#include "core\Assert.h"
#include "core\IntervalOperation.h"
#include "core\UniqueObject.h"
#include "core\StringUtils.h"
#include "core\IProgressObserver.h"
#include "core\Singleton.h"

// ----------------------------------------------------------------------------
// RTTI
// -->Core
// ----------------------------------------------------------------------------
#include "core/ReflectionObject.h"
#include "core/ReflectionType.h"
#include "core/ReflectionTypeComponent.h"
#include "core/ReflectionTypeID.h"
#include "core/ReflectionTypesRegistry.h"
#include "core/ReflectionObjectChangeListener.h"
#include "core/ReflectionDependenciesCallback.h"
#include "core/types.h"
// ----------------------------------------------------------------------------
// -->Enum
// ----------------------------------------------------------------------------
#include "core/ReflectionEnum.h"
// ----------------------------------------------------------------------------
// -->Properties
// ----------------------------------------------------------------------------
#include "core/ReflectionPropertiesView.h"
#include "core/ReflectionProperty.h"
#include "core/ReflectionPropertyEditor.h"
// ----------------------------------------------------------------------------
// -->Serialization
// ----------------------------------------------------------------------------
#include "core/ReflectionSaver.h"
#include "core/ReflectionLoader.h"
#include "core/ReflectionObjectsTracker.h"
#include "core/ReflectionSerializationUtil.h"

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
// -->DependenciesMapper
// ----------------------------------------------------------------------------
#include "core\ResourceDependenciesMapper.h"

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
#include "core\InStream.h"
#include "core\OutStream.h"
// ----------------------------------------------------------------------------
// -->Implementations
// ----------------------------------------------------------------------------
#include "core/InArrayStream.h"
#include "core/OutArrayStream.h"
#include "core/InFileStream.h"
#include "core/OutFileStream.h"

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


