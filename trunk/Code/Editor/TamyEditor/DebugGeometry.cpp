#include "DebugGeometry.h"
#include "core-Renderer\VertexShader.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Defines.h"
#include "core-Renderer\GeometryResource.h"
#include "core\ResourcesManager.h"
#include "core\FilePath.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( DebugGeometry )
   PARENT( Geometry )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

DebugGeometry::DebugGeometry( void* representedObjPtr )
   : m_representedObjPtr( representedObjPtr )
   , m_vertexShader( NULL )
{
   ResourcesManager& rm = ResourcesManager::getInstance();
   static FilePath shaderName( SHADERS_DIR "VertexShaders/staticGeometry.tvsh" );

   m_vertexShader = rm.create< VertexShader >( shaderName );
   ASSERT_MSG( m_vertexShader, "Vertex shader could not be loaded" );
}

///////////////////////////////////////////////////////////////////////////////

DebugGeometry::~DebugGeometry()
{
   m_representedObjPtr = NULL;
   m_vertexShader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& DebugGeometry::getBoundingVolume() const
{
   return getMesh()->getBoundingVolume();
}

///////////////////////////////////////////////////////////////////////////////

bool DebugGeometry::onPreRender( Renderer& renderer )
{
   if ( !m_vertexShader )
   {
      return false;
   }

   Camera& camera = renderer.getActiveCamera();

   RCBindVertexShader* comm = new ( renderer() ) RCBindVertexShader( *m_vertexShader );
   {
      const Matrix& worldMtx = getGlobalMtx();

      Matrix worldViewProjMtx;
      worldViewProjMtx.setMul( worldMtx, camera.getViewMtx() ).mul( camera.getProjectionMtx() );
      comm->setMtx( "g_mWorld", worldMtx );
      comm->setMtx( "g_mWorldViewProj", worldViewProjMtx );
   }


   return true;
}

///////////////////////////////////////////////////////////////////////////////

void DebugGeometry::onPostRender( Renderer& renderer )
{
   if ( m_vertexShader )
   {
      new ( renderer() ) RCUnbindVertexShader( *m_vertexShader );
   }
}

///////////////////////////////////////////////////////////////////////////////
