#pragma once

/// @file   core-Renderer\DynamicNodesStorage.h
/// @brief  a storage decorator that will provide a proper preprocessing to
///         dynamic nodes that should be rendered

#include "core-Renderer\RenderablesStorage.h"
#include "core\SpatialStorage.h"
#include "core\Array.h"
#include <vector>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class GraphicalNodesProcessor;
class RenderableNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * This storage will extract dynamic nodes from a proper
 * spatial storage and preprocess them so that they
 * can be introduced to the rendering pipeline.
 */
class DynamicNodesStorage : public RenderablesStorage
{
private:
   SpatialStorage<RenderableNode>* m_storage;
      
   // scene rendering helpers
   GraphicalNodesProcessor* m_nodesProcessor;
   Array<RenderableNode*> m_visibleNodes;

public:
   /**
    * Constructor.
    * 
    * @param storage    solid storage that contains the actual nodes
    *                   collection that we can render
    */
   DynamicNodesStorage(SpatialStorage<RenderableNode>* storage);
   ~DynamicNodesStorage();

   void query(Camera& camera, Array<Renderable*>& outRenderables);
};

///////////////////////////////////////////////////////////////////////////////
