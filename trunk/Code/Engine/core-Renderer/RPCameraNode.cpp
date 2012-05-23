#include "core-Renderer/RPCameraNode.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPCameraNode );
   PARENT( RenderingPipelineNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPCameraNode::RPCameraNode()
   : m_nearZ( new RPFloatOutput( "nearZ" ) )
   , m_farZ( new RPFloatOutput( "farZ" ) )
   , m_viewportWidth( new RPFloatOutput( "width" ) )
   , m_viewportHeight( new RPFloatOutput( "height" ) )
   , m_invView( new RPMatrixOutput( "invView" ) )
   , m_invProj( new RPMatrixOutput( "invProj" ) )
{
   defineOutput( m_nearZ );
   defineOutput( m_farZ );
   defineOutput( m_viewportWidth );
   defineOutput( m_viewportHeight );
   defineOutput( m_invView );
   defineOutput( m_invProj );
}

///////////////////////////////////////////////////////////////////////////////

RPCameraNode::~RPCameraNode()
{
   m_nearZ = NULL;
   m_farZ = NULL;
   m_viewportWidth = NULL;
   m_viewportHeight = NULL;
   m_invView = NULL;
   m_invProj = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPCameraNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_nearZ = DynamicCast< RPFloatOutput >( findOutput( "nearZ" ) );
   m_farZ = DynamicCast< RPFloatOutput >( findOutput( "farZ" ) );
   m_viewportWidth = DynamicCast< RPFloatOutput >( findOutput( "width" ) );
   m_viewportHeight = DynamicCast< RPFloatOutput >( findOutput( "height" ) );
   m_invView = DynamicCast< RPMatrixOutput >( findOutput( "invView" ) );
   m_invProj = DynamicCast< RPMatrixOutput >( findOutput( "invProj" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPCameraNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   Renderer& renderer = host.getRenderer();
   Camera& camera = renderer.getActiveCamera();
   RuntimeDataBuffer& data = host.data();

   m_nearZ->setValue( data, camera.getNearClippingPlane() );
   m_farZ->setValue( data, camera.getFarClippingPlane() );
   m_viewportWidth->setValue( data, (float)renderer.getViewportWidth() );
   m_viewportHeight->setValue( data, (float)renderer.getViewportHeight() );

   Matrix invMtx, transInvMtx;

   const Matrix& viewMtx = camera.getViewMtx();
   invMtx.setInverse( viewMtx );
   transInvMtx.setTransposed( invMtx );
   m_invView->setValue( data, transInvMtx );

   const Matrix& projMtx = camera.getProjectionMtx();
   invMtx.setInverse( projMtx );
   transInvMtx.setTransposed( invMtx );
   m_invProj->setValue( data, transInvMtx );
}

///////////////////////////////////////////////////////////////////////////////
