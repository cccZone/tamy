#include "core-Renderer/MNSpatialEntity.h"
#include "core-MVC/SpatialEntity.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialEntity.h"
#include "core-Renderer/Camera.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNSpatialEntity, MaterialNode )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

MNSpatialEntity::MNSpatialEntity()
   : m_worldMtx( new MSMatrixOutput( "world" ) )
   , m_worldViewMtx( new MSMatrixOutput( "worldView" ) )
{
   defineOutput( m_worldMtx );
   defineOutput( m_worldViewMtx );
}

///////////////////////////////////////////////////////////////////////////////

MNSpatialEntity::~MNSpatialEntity()
{
   m_worldMtx = NULL;
   m_worldViewMtx = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MNSpatialEntity::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_worldMtx = DynamicCast< MSMatrixOutput >( findOutput( "world" ) );
   m_worldViewMtx = DynamicCast< MSMatrixOutput >( findOutput( "worldView" ) );
}

///////////////////////////////////////////////////////////////////////////////

void MNSpatialEntity::onCreateLayout( const MaterialEntity& host ) const
{
   RuntimeDataBuffer& data = host.data();
   data.registerVar( m_parentNode );
   data[ m_parentNode ] = DynamicCast< SpatialEntity >( &host.getParent() );
}

///////////////////////////////////////////////////////////////////////////////

void MNSpatialEntity::preRender( Renderer& renderer, const MaterialEntity& entity ) const
{
   RuntimeDataBuffer& data = entity.data();
   SpatialEntity* node = data[ m_parentNode ];

   if ( node )
   {
      Camera& camera = renderer.getActiveCamera();

      const D3DXMATRIX& worldMtx = node->getGlobalMtx();
      m_worldMtx->setValue( data, worldMtx );

      D3DXMATRIX worldView = worldMtx * camera.getViewMtx();
      m_worldViewMtx->setValue( data, worldView );
   }
}

///////////////////////////////////////////////////////////////////////////////
