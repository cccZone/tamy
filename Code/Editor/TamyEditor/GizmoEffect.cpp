#include "GizmoEffect.h"
#include "core.h"
#include "core-Renderer.h"


///////////////////////////////////////////////////////////////////////////////

GizmoEffect::GizmoEffect( ResourcesManager& rm, Camera& camera )
: m_camera( camera )
, m_renderedNode( NULL )
{
   Shader* shader = dynamic_cast< Shader*>( rm.findResource( "GizmoEffect" ) );

   if ( !shader )
   {
      // load the shader's code
      const Filesystem& fs = rm.getFilesystem();
      std::string shaderFileName = "/Editor/Shaders/GizmoEffect.fx";
      File* shaderFile = fs.open( shaderFileName );

      StreamBuffer< char > fileReader( *shaderFile );
      std::string shaderCode = fileReader.getBuffer();

      // cleanup
      delete shaderFile;

      // create the shader 
      shader = new Shader( "GizmoEffect", shaderCode );
      rm.addResource( shader );
   }

   // create the effect
   initialize( *shader );
} 

///////////////////////////////////////////////////////////////////////////////

void GizmoEffect::onAttached(Entity& parent)
{
   m_renderedNode = dynamic_cast< SpatialEntity* >( &parent );
}

///////////////////////////////////////////////////////////////////////////////

void GizmoEffect::onDetached(Entity& parent)
{
   m_renderedNode = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GizmoEffect::onBeginRendering()
{
   if ( !m_renderedNode )
   {
      return;
   }

   D3DXMATRIX worldViewProjMtx = m_renderedNode->getGlobalMtx() 
      * m_camera.getViewMtx() 
      * m_camera.getProjectionMtx();
   shader().setMtx("g_mWorldViewProj", worldViewProjMtx);

   shader().setTechnique("tec0");
}

///////////////////////////////////////////////////////////////////////////////
