#include "core-Renderer\RenderResource.h"
#include "core-Renderer\Renderer.h"


///////////////////////////////////////////////////////////////////////////////

int RenderResource::getRenderResourceId( const Renderer& renderer )
{
   unsigned int rendererId = renderer.getIndex();
   if ( rendererId >= m_resourceId.size() )
   {
      m_resourceId.resize( rendererId + 1, -1 );
   }

   return m_resourceId[ rendererId ];
}

///////////////////////////////////////////////////////////////////////////////

void RenderResource::setRenderResourceId( const Renderer& renderer, int id )
{
   unsigned int rendererId = renderer.getIndex();
   if ( rendererId >= m_resourceId.size() )
   {
      m_resourceId.resize( rendererId + 1, -1 );
   }

   m_resourceId[ rendererId ] = id;
}

///////////////////////////////////////////////////////////////////////////////
