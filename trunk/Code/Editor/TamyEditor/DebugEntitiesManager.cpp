#include "DebugEntitiesManager.h"
#include "EditorDebugRenderer.h"
#include "core-Renderer.h"

// debug representations
#include "DRDirectionalLight.h"
#include "DRPointLight.h"


///////////////////////////////////////////////////////////////////////////////

DebugEntitiesManager::DebugEntitiesManager( EditorDebugRenderer& debugRenderer )
   : m_debugRenderer( debugRenderer )
{
   associate< DirectionalLight, DRDirectionalLight >();
   associate< PointLight, DRPointLight >();
}

///////////////////////////////////////////////////////////////////////////////

DebugEntitiesManager::~DebugEntitiesManager()
{
   resetContents();
}

///////////////////////////////////////////////////////////////////////////////

void DebugEntitiesManager::update( float timeElapsed )
{
   uint count = m_representations.size();
   for ( uint i = 0; i < count; ++i )
   {
      const Matrix& globalMtx = m_representedEntities[i]->getGlobalMtx();
      m_representations[i]->setLocalMtx( globalMtx );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DebugEntitiesManager::onEntityAdded( Entity& entity )
{
   if ( !entity.isA< SpatialEntity >() )
   {
      return;
   }

   SpatialEntity* spatialEntity = static_cast< SpatialEntity* >( &entity );
   // check uniqueness
   uint count = m_representedEntities.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_representedEntities[i] == spatialEntity )
      {
         // already got it
         return;
      }
   }

   DebugGeometry* representation = create( *spatialEntity );
   if ( representation )
   {
      m_debugRenderer.add( *representation );
      m_representedEntities.push_back( spatialEntity );
      m_representations.push_back( representation );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DebugEntitiesManager::onEntityRemoved( Entity& entity )
{
   uint count = m_representedEntities.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_representedEntities[i] == &entity )
      {
         // found it
         m_representedEntities.erase( m_representedEntities.begin() + i );

         DebugGeometry* representation = m_representations[i];
         m_debugRenderer.remove( *representation );
         delete representation;
         m_representations.erase( m_representations.begin() + i );

         return;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void DebugEntitiesManager::onEntityChanged( Entity& entity )
{
   // nothing to do here
}

///////////////////////////////////////////////////////////////////////////////

void DebugEntitiesManager::resetContents()
{
   uint count = m_representations.size();
   for ( uint i = 0; i < count; ++i )
   {
      m_debugRenderer.remove( *m_representations[i] );
      delete m_representations[i];
   }
   m_representations.clear();

   m_representedEntities.clear();
}

///////////////////////////////////////////////////////////////////////////////
