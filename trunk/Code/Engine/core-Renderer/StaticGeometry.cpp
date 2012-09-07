#include "core-Renderer\StaticGeometry.h"
#include "core-MVC\SpatialEntity.h"
#include "core-Renderer\VertexShader.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Defines.h"
#include "core-MVC.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( StaticGeometry )
   PARENT( Geometry )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

StaticGeometry::StaticGeometry( const char* name )
   : Geometry( name != NULL ? name : "" )
   , m_vertexShader( NULL )
{
   initialize();
}

///////////////////////////////////////////////////////////////////////////////

StaticGeometry::StaticGeometry( GeometryResource& resource, const char* name  )
   : Geometry( resource, name != NULL ? name : "" )
   , m_vertexShader( NULL )
{
   initialize();
}

///////////////////////////////////////////////////////////////////////////////

StaticGeometry::~StaticGeometry()
{
}

///////////////////////////////////////////////////////////////////////////////

RCBindVertexShader* StaticGeometry::onPreRender( Renderer& renderer )
{
   if ( !m_vertexShader )
   {
      return NULL;
   }

   Camera& camera = renderer.getActiveCamera();

   RCBindVertexShader* comm = new ( renderer() ) RCBindVertexShader( *m_vertexShader, renderer );
   {
      const Matrix& worldMtx = getGlobalMtx();

      Matrix worldViewMtx;
      worldViewMtx.setMul( worldMtx, camera.getViewMtx() );

      Matrix worldViewProjMtx;
      worldViewProjMtx.setMul( worldViewMtx, camera.getProjectionMtx() );

      comm->setMtx( "g_mWorldView", worldViewMtx );
      comm->setMtx( "g_mWorldViewProj", worldViewProjMtx );
   }

   return comm;
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

void StaticGeometry::initialize()
{
   ResourcesManager& rm = ResourcesManager::getInstance();
   static FilePath shaderName( SHADERS_DIR "VertexShaders/staticGeometry.tvsh" );

   m_vertexShader = rm.create< VertexShader >( shaderName );
   ASSERT_MSG( m_vertexShader, "Vertex shader could not be loaded" );
}

///////////////////////////////////////////////////////////////////////////////

Entity* StaticGeometry::cloneSelf() const
{
   StaticGeometry* entity = new StaticGeometry( *this );
   return entity;
}

///////////////////////////////////////////////////////////////////////////////
