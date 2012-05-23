#include "core-Renderer/GNSpatialEntity.h"
#include "core-MVC/SpatialEntity.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/GeometrySockets.h"
#include "core-Renderer/GeometryEntity.h"
#include "core-Renderer/Camera.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GNSpatialEntity );
   PARENT( GeometryShaderNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GNSpatialEntity::GNSpatialEntity()
   : m_worldMtx( new GSMatrixOutput( "world" ) )
   , m_worldViewMtx( new GSMatrixOutput( "worldView" ) )
{
   defineOutput( m_worldMtx );
   defineOutput( m_worldViewMtx );
}

///////////////////////////////////////////////////////////////////////////////

GNSpatialEntity::~GNSpatialEntity()
{
   m_worldMtx = NULL;
   m_worldViewMtx = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GNSpatialEntity::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_worldMtx = DynamicCast< GSMatrixOutput >( findOutput( "world" ) );
   m_worldViewMtx = DynamicCast< GSMatrixOutput >( findOutput( "worldView" ) );
}

///////////////////////////////////////////////////////////////////////////////

void GNSpatialEntity::onCreateLayout( const GeometryEntity& host ) const
{
   RuntimeDataBuffer& data = host.data();
   data.registerVar( m_parentNode );
   data[ m_parentNode ] = DynamicCast< SpatialEntity >( &host.getParent() );
}

///////////////////////////////////////////////////////////////////////////////

void GNSpatialEntity::preRender( Renderer& renderer, const GeometryEntity& entity ) const
{
   RuntimeDataBuffer& data = entity.data();
   SpatialEntity* node = data[ m_parentNode ];

   if ( node )
   {
      Camera& camera = renderer.getActiveCamera();

      const Matrix& worldMtx = node->getGlobalMtx();
      m_worldMtx->setValue( data, worldMtx );

      Matrix worldView;
      worldView.setMul( worldMtx, camera.getViewMtx() );
      m_worldViewMtx->setValue( data, worldView );
   }
}

///////////////////////////////////////////////////////////////////////////////
