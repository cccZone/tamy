#include "core-Renderer/MNCamera.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialInstance.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNCamera );
   PARENT( MaterialNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MNCamera::MNCamera()
   : m_nearZ( new MSFloatOutput( "nearZ" ) )
   , m_farZ( new MSFloatOutput( "farZ" ) )
   , m_viewportWidth( new MSFloatOutput( "width" ) )
   , m_viewportHeight( new MSFloatOutput( "height" ) )
   , m_invView( new MSMatrixOutput( "invView" ) )
   , m_invProj( new MSMatrixOutput( "invProj" ) )
{
   defineOutput( m_nearZ );
   defineOutput( m_farZ );
   defineOutput( m_viewportWidth );
   defineOutput( m_viewportHeight );
   defineOutput( m_invView );
   defineOutput( m_invProj );
}

///////////////////////////////////////////////////////////////////////////////

MNCamera::~MNCamera()
{
   m_nearZ = NULL;
   m_farZ = NULL;
   m_viewportWidth = NULL;
   m_viewportHeight = NULL;
   m_invView = NULL;
   m_invProj = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MNCamera::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_nearZ = DynamicCast< MSFloatOutput >( findOutput( "nearZ" ) );
   m_farZ = DynamicCast< MSFloatOutput >( findOutput( "farZ" ) );
   m_viewportWidth = DynamicCast< MSFloatOutput >( findOutput( "width" ) );
   m_viewportHeight = DynamicCast< MSFloatOutput >( findOutput( "height" ) );
   m_invView = DynamicCast< MSMatrixOutput >( findOutput( "invView" ) );
   m_invProj = DynamicCast< MSMatrixOutput >( findOutput( "invProj" ) );
}

///////////////////////////////////////////////////////////////////////////////

void MNCamera::preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const
{
   Camera& camera = renderer.getActiveCamera();

   RuntimeDataBuffer& data = instance.data();
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
