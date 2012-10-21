/// @file   core-Renderer/RenderResource.h
/// @brief  a resource managed by the renderer
#pragma once

#include <vector>
#include "core\MemoryRouter.h"


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
   DECLARE_ALLOCATOR( RenderResource, AM_DEFAULT );

private:
   std::vector< IRenderResourceStorage* >    m_hostStorage;
   std::vector< int >                        m_resourceId;
   std::vector< bool >                       m_dirtyFlag;
   mutable std::vector< bool >               m_freshDataNeededFlag;

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

   // -------------------------------------------------------------------------
   // Runtime change management
   // -------------------------------------------------------------------------
   /**
    * [Application ---> Implementation]
    * Checks if the resource changed and its particular implementation needs to be refreshed.
    *
    * @param renderer
    */
   bool isDirty( const Renderer& renderer );

   /**
    * [Implementation ---> Application] 
    * Marks the resource refreshed after one of its implementation's contents were destroyed.
    *
    * @param renderer
    */
   void markRefreshed( Renderer& renderer );

   /**
    * [Implementation ---> Application] 
    * Checks if the particular buffer needs its data refreshed.
    *
    * Some may have had their implementations destroyed when a device was lost etc.
    * and ones containing rarely refreshed data ( such as lightmaps or material description textures )
    * will need to be refreshed once that happens
    */
   bool needsRefreshing( Renderer& renderer );

   /**
    * [Implementation ---> Application] 
    * Communicates the fact that one of the render target's implementations was recreated
    * and probably needs to be refreshed ( if anyone cares ).
    */
   void markNotFresh( const Renderer& renderer ) const;


protected:
   /**
    * [Application ---> Implementation]  
    * Indicates that the resource changed and the implementations should be refreshed.
    */
   void setDirty();
};

///////////////////////////////////////////////////////////////////////////////
