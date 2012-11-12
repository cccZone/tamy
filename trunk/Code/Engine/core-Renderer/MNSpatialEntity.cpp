#include "core-Renderer/MNSpatialEntity.h"
#include "core-MVC/SpatialEntity.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialEntity.h"
#include "core-Renderer/MaterialInstance.h"
#include "core-Renderer/Camera.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNSpatialEntity );
   PARENT( MaterialNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MNSpatialEntity::MNSpatialEntity()
   : m_worldMtx( NULL )
   , m_worldViewMtx( NULL )
{
   bool isBeingDeserialized = SerializationFlag::getInstance().isSerializationInProgress();
   if ( !isBeingDeserialized )
   {
      m_worldMtx = new MSMatrixOutput( "world" );
      m_worldViewMtx = new MSMatrixOutput( "worldView" );

      std::vector< GBNodeOutput< MaterialNode >* > outputs;
      outputs.push_back( m_worldMtx );
      outputs.push_back( m_worldViewMtx );

      defineOutputs( outputs );
   }
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

void MNSpatialEntity::onCreateLayout( const MaterialInstance& host ) const
{
   RuntimeDataBuffer& data = host.data();
   data.registerVar( m_parentNode );
   data[ m_parentNode ] = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MNSpatialEntity::preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const
{
   RuntimeDataBuffer& data = instance.data();
   SpatialEntity* parentNode = data[ m_parentNode ];
   if ( !parentNode )
   {
      // initialize the parent entity if it wasn't initialized yet
      parentNode = DynamicCast< SpatialEntity >( entity.getParent() );
      data[ m_parentNode ] = parentNode;
   }

   if ( parentNode )
   {
      Camera& camera = renderer.getActiveCamera();

      const Matrix& worldMtx = parentNode->getGlobalMtx();
      m_worldMtx->setValue( data, worldMtx );

      Matrix worldView;
      worldView.setMul( worldMtx, camera.getViewMtx() );
      m_worldViewMtx->setValue( data, worldView );
   }
}

///////////////////////////////////////////////////////////////////////////////
