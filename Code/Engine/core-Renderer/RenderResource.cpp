#include "core-Renderer\RenderResource.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

RenderResource::~RenderResource()
{
   int count = m_hostStorage.size();
   for ( int i = 0; i < count; ++i )
   {
      if ( m_hostStorage[i] )
      {
         m_hostStorage[i]->removeResource( m_resourceId[i] );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

int RenderResource::getRenderResourceId( const Renderer& renderer )
{
   unsigned int rendererId = renderer.getIndex();
   if ( rendererId >= m_resourceId.size() )
   {
      m_resourceId.resize( rendererId + 1, -1 );
      m_hostStorage.resize( rendererId + 1, NULL );
   }

   return m_resourceId[ rendererId ];
}

///////////////////////////////////////////////////////////////////////////////

void RenderResource::setRenderResourceId( const Renderer& renderer, IRenderResourceStorage& hostStorage, int id )
{
   unsigned int rendererId = renderer.getIndex();
   if ( rendererId >= m_resourceId.size() )
   {
      m_resourceId.resize( rendererId + 1, -1 );
      m_hostStorage.resize( rendererId + 1, NULL );
   }

   m_hostStorage[ rendererId ] = &hostStorage;
   m_resourceId[ rendererId ] = id;
}

///////////////////////////////////////////////////////////////////////////////

void RenderResource::onRenderResourceReleased( const Renderer& renderer )
{
   unsigned int rendererId = renderer.getIndex();

   if ( rendererId < m_resourceId.size() )
   {
      m_resourceId[rendererId] = -1;
      m_hostStorage[rendererId] = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
