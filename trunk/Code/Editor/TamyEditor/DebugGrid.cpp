#include "DebugGrid.h"
#include "core-Renderer/LineSegments.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/BasicRenderCommands.h"
#include "core-Renderer/LitVertex.h"
#include "core-Renderer/VertexShader.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/Defines.h"
#include "DebugGeometryBuilder.h"
#include "SingleColorDebugMat.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT(DebugGrid)
   PARENT( DebugGeometry )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

DebugGrid::DebugGrid()
   : DebugGeometry( NULL )
{
   Color gridColor( 100, 100, 255 );
   // setup the material
   {
      m_material = new SingleColorDebugMat( gridColor );
      addState( *m_material );
   }

   // setup grid geometry
   {   
      m_mesh = DebugGeometryBuilder::createGrid( 1000, 50, gridColor );
      setMesh( *m_mesh );
   }

   // load the vertex shader
   {
      ResourcesManager& rm = ResourcesManager::getInstance();
      static FilePath shaderName( SHADERS_DIR "VertexShaders/linearGeometry.tvsh" );

      m_vertexShader = rm.create< VertexShader >( shaderName );
      ASSERT_MSG( m_vertexShader, "Vertex shader could not be loaded" );
   }
}

///////////////////////////////////////////////////////////////////////////////

DebugGrid::~DebugGrid()
{
   delete m_material;
   m_material = NULL;

   delete m_mesh;
   m_mesh = NULL;
}

///////////////////////////////////////////////////////////////////////////////

RCBindVertexShader* DebugGrid::onPreRender( Renderer& renderer )
{
   if ( !m_vertexShader )
   {
      return NULL;
   }

   Camera& camera = renderer.getActiveCamera();

   // setup the vertex shader
   RCBindVertexShader* comm = new ( renderer() ) RCBindVertexShader( *m_vertexShader, renderer );
   {
      Matrix worldViewProjMtx;
      worldViewProjMtx.setMul( camera.getViewMtx(), camera.getProjectionMtx() );
      comm->setMtx( "g_mWorldViewProj", worldViewProjMtx );
   }

   return comm;
}

///////////////////////////////////////////////////////////////////////////////

void DebugGrid::onPostRender( Renderer& renderer )
{
   if ( m_vertexShader )
   {
      new ( renderer() ) RCUnbindVertexShader( *m_vertexShader );
   }
}

///////////////////////////////////////////////////////////////////////////////
