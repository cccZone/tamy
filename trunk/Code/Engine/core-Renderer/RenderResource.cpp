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
      m_dirtyFlag.resize( rendererId + 1, false );
      m_freshDataNeededFlag.resize( rendererId + 1, false );
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

      // we're not setting the dirty flag at the beginning - it's a user mechanism, not something mandatory,
      // and besides, each newly created implementation should initialize itself at start
      m_dirtyFlag.resize( rendererId + 1, false );

      m_freshDataNeededFlag.resize( rendererId + 1, false );
   }

   m_hostStorage[ rendererId ] = &hostStorage;
   m_resourceId[ rendererId ] = id;
   m_dirtyFlag[ rendererId ] = false;

   // the first time we create a resource, it always needs fresh data
   m_freshDataNeededFlag[ rendererId ] = true;
}

///////////////////////////////////////////////////////////////////////////////

void RenderResource::onRenderResourceReleased( const Renderer& renderer )
{
   unsigned int rendererId = renderer.getIndex();

   if ( rendererId < m_resourceId.size() )
   {
      m_resourceId[rendererId] = -1;
      m_hostStorage[rendererId] = NULL;
      m_dirtyFlag[rendererId] = false;
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderResource::setDirty()
{
   unsigned int count = m_dirtyFlag.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_dirtyFlag[i] = true;
   }
}

///////////////////////////////////////////////////////////////////////////////

bool RenderResource::isDirty( const Renderer& renderer )
{
   unsigned int rendererId = renderer.getIndex();

   if ( rendererId < m_resourceId.size() )
   {
      return m_dirtyFlag[rendererId];
   }
   else
   {
      return false;
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderResource::markRefreshed( Renderer& renderer )
{
   unsigned int rendererId = renderer.getIndex();
   if ( rendererId < m_resourceId.size() )
   {
      m_freshDataNeededFlag[rendererId] = false;
   }
}

///////////////////////////////////////////////////////////////////////////////

bool RenderResource::needsRefreshing( Renderer& renderer )
{
   unsigned int rendererId = renderer.getIndex();
   if ( rendererId < m_resourceId.size() )
   {
      return m_freshDataNeededFlag[rendererId];
   }
   else
   {
      // at this point the resource doesn't have an implementation,
      // so there's no point in 'refreshing' its contents
      return false;
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderResource::markNotFresh( const Renderer& renderer ) const
{
   unsigned int rendererId = renderer.getIndex();
   if ( rendererId < m_resourceId.size() )
   {
      m_freshDataNeededFlag[rendererId] = true;
   }
}

///////////////////////////////////////////////////////////////////////////////
