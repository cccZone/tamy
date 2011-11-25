#include "core-Renderer/MNSpatialEntity.h"
#include "core-MVC/SpatialEntity.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/MaterialSockets.h"


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
/*
void MNSpatialEntity::onUpdate( RenderingPipelineMechanism& host ) const
{
   if ( !m_parentNode )
   {
      return;
   }
   Renderer& renderer = host.getRenderer();
   Camera& camera = renderer.getActiveCamera();
   RuntimeDataBuffer& data = host.data();

   const D3DXMATRIX& worldMtx = m_parentNode->getGlobalMtx();
   m_worldMtx->setValue( data, worldMtx );

   D3DXMATRIX worldView = worldMtx * camera.getViewMtx();
   m_worldViewMtx->setValue( data, worldView );
}

///////////////////////////////////////////////////////////////////////////////
*/