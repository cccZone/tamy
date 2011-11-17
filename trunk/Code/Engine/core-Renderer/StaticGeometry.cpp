#include "core-Renderer\StaticGeometry.h"
#include "core-MVC\SpatialEntity.h"
#include "core-Renderer\VertexShader.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Defines.h"
#include "core-MVC.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( StaticGeometry, Geometry )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

StaticGeometry::StaticGeometry()
   : Geometry()
   , m_vertexShader( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

StaticGeometry::StaticGeometry( GeometryResource& resource )
   : Geometry( resource )
   , m_vertexShader( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

StaticGeometry::~StaticGeometry()
{
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometry::onPreRender( Renderer& renderer )
{
   if ( !m_vertexShader )
   {
      return;
   }

   Camera& camera = renderer.getActiveCamera();

   RCBindVertexShader* comm = new ( renderer() ) RCBindVertexShader( *m_vertexShader );
   D3DXMATRIX worldViewMtx = getGlobalMtx() * camera.getViewMtx();
   comm->setMtx( "g_mWorldView", worldViewMtx );
   comm->setMtx( "g_mProjection", camera.getProjectionMtx() );
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometry::onPostRender( Renderer& renderer )
{
   if ( !m_vertexShader )
   {
      return;
   }

   new ( renderer() ) RCUnbindVertexShader( *m_vertexShader );
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometry::onComponentAdded( Component< Model >& component )
{
   __super::onComponentAdded( component );

   ModelComponent< ResourcesManager >* comp = dynamic_cast< ModelComponent< ResourcesManager >* >( &component );
   if ( comp )
   {
      // load the shader
      ResourcesManager& rm = comp->get();
      static const char* shaderName = SHADERS_DIR "StaticGeometry.vsh";
      m_vertexShader = rm.findResource< VertexShader >( shaderName );
      if ( !m_vertexShader )
      {
         m_vertexShader = new VertexShader( shaderName );
         m_vertexShader->setVertexDescription( VDI_SIMPLE );
         rm.addResource( m_vertexShader );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
