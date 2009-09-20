#ifndef _SORTING_RENDERABLES_STORAGE_H
#define _SORTING_RENDERABLES_STORAGE_H

/// @file   core-Renderer\SortingRenderablesStorage.h
/// @brief  a storage adapter that will provide a proper preprocessing to
///         dynamic nodes that should be rendered

#include "core-Renderer\RenderablesStorage.h"
#include "core-Renderer\RenderablesSorter.h"
#include "core\SpatialStorage.h"
#include "core\Array.h"
#include <vector>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class RenderableNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * This storage will extract dynamic nodes from a proper
 * spatial storage and preprocess them so that they
 * can be introduced to the rendering pipeline.
 */
template<typename InRenderable>
class SortingRenderablesStorage : public RenderablesStorage
{
private:
   typename SpatialStorage<InRenderable>& m_storage;
      
   // scene rendering helpers
   typename RenderablesSorter<InRenderable>* m_nodesProcessor;
   typename Array<InRenderable*> m_visibleNodes;

public:
   /**
    * Constructor.
    * 
    * @param storage    solid storage that contains the actual nodes
    *                   collection that we can render
    */
   SortingRenderablesStorage(typename SpatialStorage<InRenderable>& storage);
   ~SortingRenderablesStorage();

   void query(Camera& camera, Array<Renderable*>& outRenderables);
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\SortingRenderablesStorage.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _SORTING_RENDERABLES_STORAGE_H
