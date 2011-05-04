#include "core-Renderer\StaticGeometry.h"
#include "core-MVC\SpatialEntity.h"
#include "core-Renderer\VertexShader.h"
#include "core-Renderer\Camera.h"
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

void StaticGeometry::onPreRender()
{
   if ( !m_vertexShader )
   {
      return;
   }

   D3DXMATRIX worldViewMtx = m_parentNode->getGlobalMtx() * m_camera->getViewMtx();
   m_vertexShader->setMtx( "g_mWorldView", worldViewMtx );
   m_vertexShader->setMtx( "g_mProjection", m_camera->getProjectionMtx() );
   m_vertexShader->beginRendering();
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometry::onPostRender()
{
   if ( m_vertexShader )
   {
      m_vertexShader->endRendering();
   }
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
      static const char* shaderName = "Renderer/Shaders/StaticGeometry.vsh";
      m_vertexShader = dynamic_cast< VertexShader* >( rm.findResource( shaderName ) );
      if ( !m_vertexShader )
      {
         m_vertexShader = new VertexShader( shaderName );
         m_vertexShader->setVertexDescription( VDI_SIMPLE );
         rm.addResource( m_vertexShader );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
