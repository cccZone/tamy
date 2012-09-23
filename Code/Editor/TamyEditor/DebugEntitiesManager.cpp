#include "DebugEntitiesManager.h"
#include "EditorDebugRenderer.h"
#include "core-Renderer.h"
#include "core.h"

// debug representations
#include "DebugGrid.h"
#include "DRDirectionalLight.h"
#include "DRPointLight.h"
#include "DRGeometry.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ENUM( DebugFeature );
   ENUM_VAL( BoundingBoxes );
   ENUM_VAL( DebugShapes );
END_ENUM();

///////////////////////////////////////////////////////////////////////////////

DebugEntitiesManager::DebugEntitiesManager( EditorDebugRenderer& debugRenderer )
   : m_debugRenderer( debugRenderer )
   , m_debugGrid( new DebugGrid() )
{
   debugRenderer.setHost( *this );

   // set debug features
   m_debugFeatureEnabled = new bool[MAX_DEBUG_FEATURES];
   memset( m_debugFeatureEnabled, 0, sizeof( bool ) * MAX_DEBUG_FEATURES );

   m_debugFeatureEnabled[DebugShapes] = true;

   // register debug representations
   associate< DirectionalLight, DRDirectionalLight >();
   associate< PointLight, DRPointLight >();
   associateAbstract< Geometry, DRGeometry >();
}

///////////////////////////////////////////////////////////////////////////////

DebugEntitiesManager::~DebugEntitiesManager()
{
   resetContents();

   delete m_debugGrid;
   m_debugGrid = NULL;

   delete [] m_debugFeatureEnabled;
   m_debugFeatureEnabled = NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool DebugEntitiesManager::toggleDebugDisplay( DebugFeature flag )
{
   m_debugFeatureEnabled[flag] = !m_debugFeatureEnabled[flag];

   // apply the feature to the existing representations
   uint count = m_representations.size();
   for ( uint i = 0; i < count; ++i )
   {
      DebugGeometry* representation = m_representations[i];
      applyFeature( representation, flag );
   }

   return m_debugFeatureEnabled[flag];
}

///////////////////////////////////////////////////////////////////////////////

void DebugEntitiesManager::disableAll()
{
   memset( m_debugFeatureEnabled, 0, sizeof( bool ) * MAX_DEBUG_FEATURES );

   uint count = m_representations.size();
   for ( uint i = 0; i < count; ++i )
   {
      DebugGeometry* representation = m_representations[i];

      for ( uint featIdx = 0; featIdx < MAX_DEBUG_FEATURES; ++featIdx )
      {
         applyFeature( representation, (DebugFeature)featIdx );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void DebugEntitiesManager::showGrid( bool enable )
{
   m_debugGrid->show( enable );
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

      // apply all debug features to it
      for ( uint i = 0; i < MAX_DEBUG_FEATURES; ++i )
      {
         applyFeature( representation, (DebugFeature)i );
      }
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

void DebugEntitiesManager::applyFeature( DebugGeometry* representation, DebugFeature feature )
{
   bool enable = m_debugFeatureEnabled[feature];

   switch( feature )
   {
   case BoundingBoxes:
      {
         representation->enableBoundingBox( enable );
         break;
      }

   case DebugShapes:
      {
         representation->enableDebugShape( enable );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void DebugEntitiesManager::onDebugSceneCreated()
{
   // add debug grid
   m_debugRenderer.add( *m_debugGrid );

}

///////////////////////////////////////////////////////////////////////////////

void DebugEntitiesManager::onDebugSceneDestroyed()
{
   // remove debug grid
   m_debugRenderer.remove( *m_debugGrid );
}

///////////////////////////////////////////////////////////////////////////////
