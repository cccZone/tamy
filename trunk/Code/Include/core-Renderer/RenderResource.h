/// @file   core-Renderer/RenderResource.h
/// @brief  a resource managed by the renderer
#pragma once

#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class IRenderResourceStorage;

///////////////////////////////////////////////////////////////////////////////

/**
 * Renderer needs to keep its own representations of the rendered stuff ( such as
 * vertex & index buffers, textures etc. ).
 *
 * This class will allow to simply reference those internal renderer resources.
 */
class RenderResource
{
private:
   std::vector< IRenderResourceStorage* >    m_hostStorage;
   std::vector< int >                        m_resourceId;

public:
   virtual ~RenderResource();

   /**
    * Returns a resource id identifying a resource managed by the specified renderer.
    *
    * @param renderer
    */
   int getRenderResourceId( const Renderer& renderer );

   /**
    * Assigns an id of the resource managed by the specified renderer.
    *
    * @param renderer
    * @param hostStorage
    * @param id
    */
   void setRenderResourceId( const Renderer& renderer, IRenderResourceStorage& hostStorage, int id );

   /**
    * Called when the resource is being flushed out from the storage ( i.e. when the renderer is being deleted )
    */
   void onRenderResourceReleased( const Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
