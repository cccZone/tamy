#include "core-Renderer/GNCamera.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/GeometrySockets.h"
#include "core-Renderer/GeometryEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GNCamera, GeometryShaderNode )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

GNCamera::GNCamera()
   : m_nearZ( new GSFloatOutput( "nearZ" ) )
   , m_farZ( new GSFloatOutput( "farZ" ) )
   , m_viewportWidth( new GSFloatOutput( "width" ) )
   , m_viewportHeight( new GSFloatOutput( "height" ) )
   , m_view( new GSMatrixOutput( "view" ) )
   , m_proj( new GSMatrixOutput( "proj" ) )
   , m_worldView( new GSMatrixOutput( "worldView" ) )
   , m_worldViewProj( new GSMatrixOutput( "worldViewProj" ) )
{
   defineOutput( m_nearZ );
   defineOutput( m_farZ );
   defineOutput( m_viewportWidth );
   defineOutput( m_viewportHeight );
   defineOutput( m_view );
   defineOutput( m_proj );
   defineOutput( m_worldView );
   defineOutput( m_worldViewProj );
}

///////////////////////////////////////////////////////////////////////////////

GNCamera::~GNCamera()
{
   m_nearZ = NULL;
   m_farZ = NULL;
   m_viewportWidth = NULL;
   m_viewportHeight = NULL;
   m_view = NULL;
   m_proj = NULL;
   m_worldView = NULL;
   m_worldViewProj = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GNCamera::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_nearZ = DynamicCast< GSFloatOutput >( findOutput( "nearZ" ) );
   m_farZ = DynamicCast< GSFloatOutput >( findOutput( "farZ" ) );
   m_viewportWidth = DynamicCast< GSFloatOutput >( findOutput( "width" ) );
   m_viewportHeight = DynamicCast< GSFloatOutput >( findOutput( "height" ) );
   m_view = DynamicCast< GSMatrixOutput >( findOutput( "view" ) );
   m_proj = DynamicCast< GSMatrixOutput >( findOutput( "proj" ) );
   m_worldView = DynamicCast< GSMatrixOutput >( findOutput( "worldView" ) );
   m_worldViewProj = DynamicCast< GSMatrixOutput >( findOutput( "worldViewProj" ) );
}

///////////////////////////////////////////////////////////////////////////////

void GNCamera::preRender( Renderer& renderer, const GeometryEntity& entity ) const
{
   Camera& camera = renderer.getActiveCamera();

   RuntimeDataBuffer& data = entity.data();
   m_nearZ->setValue( data, camera.getNearClippingPlane() );
   m_farZ->setValue( data, camera.getFarClippingPlane() );
   m_viewportWidth->setValue( data, renderer.getViewportWidth() );
   m_viewportHeight->setValue( data, renderer.getViewportHeight() );

   const D3DXMATRIX& viewMtx = camera.getViewMtx();
   m_view->setValue( data, viewMtx );

   const D3DXMATRIX& projMtx = camera.getProjectionMtx();
   m_proj->setValue( data, projMtx );

   const D3DXMATRIX& worldMtx = entity.getGlobalMtx();

   D3DXMATRIX worldViewMtx = worldMtx * viewMtx;
   m_worldView->setValue( data, worldViewMtx );

   D3DXMATRIX worldViewProjMtx = worldViewMtx * projMtx;
   m_worldViewProj->setValue( data, worldViewProjMtx );
}

///////////////////////////////////////////////////////////////////////////////
