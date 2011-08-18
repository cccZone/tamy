/// @file   core-Renderer/RenderResource.h
/// @brief  a resource managed by the renderer
#pragma once

#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Renderer;

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
   std::vector< int >         m_resourceId;

public:

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
    * @param id
    */
   void setRenderResourceId( const Renderer& renderer, int id );
};

///////////////////////////////////////////////////////////////////////////////
