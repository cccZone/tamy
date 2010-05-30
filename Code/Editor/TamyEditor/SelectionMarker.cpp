#include "SelectionMarker.h"
#include "core.h"
#include "core-Renderer.h"


///////////////////////////////////////////////////////////////////////////////

SelectionMarker::SelectionMarker( ResourcesManager& rm, Camera& camera, Node& renderedNode )
: m_camera( camera )
, m_renderedNode( renderedNode )
{
   Shader* shader = dynamic_cast< Shader*>( rm.findResource( "SelectionMarker" ) );

   if ( !shader )
   {
      // load the shader's code
      const Filesystem& fs = rm.getFilesystem();
      std::string shaderFileName = "/Editor/Shaders/SelectionMarker.fx";
      File* shaderFile = fs.open( shaderFileName );

      StreamBuffer< char > fileReader( *shaderFile );
      std::string shaderCode = fileReader.getBuffer();

      // cleanup
      delete shaderFile;

      // create the shader 
      shader = new Shader( "SelectionMarker", shaderCode );
      rm.addResource( shader );
   }

   // create the effect
   initialize( *shader );
} 

///////////////////////////////////////////////////////////////////////////////

void SelectionMarker::onBeginRendering()
{
   D3DXMATRIX worldViewProjMtx = m_renderedNode.getGlobalMtx() 
      * m_camera.getViewMtx() 
      * m_camera.getProjectionMtx();
   shader().setMtx("g_mWorldViewProj", worldViewProjMtx);

   shader().setTechnique("tec0");
}

///////////////////////////////////////////////////////////////////////////////
