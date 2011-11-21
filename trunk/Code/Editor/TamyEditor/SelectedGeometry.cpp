#include "SelectedGeometry.h"
#include "core-Renderer.h"
#include "SelectionManager.h"
#include "tamyeditor.h"


///////////////////////////////////////////////////////////////////////////////

SelectedGeometry::SelectedGeometry( Geometry& geometry )
: m_geometry( geometry )
, m_selectionMarker( NULL )
{
   // create a selection marker effect
   static FilePath shaderName( "Editor/Shaders/SelectionMarker.psh" );
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   m_selectionMarker = resMgr.findResource< PixelShader >( shaderName );
   if ( !m_selectionMarker )
   {
      m_selectionMarker = new PixelShader( shaderName );

      // load the shader code
      File* shaderFile = resMgr.getFilesystem().open( shaderName, std::ios_base::in );
      StreamBuffer< char > shaderCodeBuf( *shaderFile );
      m_selectionMarker->setScript( shaderCodeBuf.getBuffer() );
      delete shaderFile;

      resMgr.addResource( m_selectionMarker );
   }
}

///////////////////////////////////////////////////////////////////////////////

SelectedGeometry::~SelectedGeometry()
{
   m_selectionMarker = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SelectedGeometry::render( Renderer& renderer )
{
   if ( !m_geometry.hasGeometry() )
   {
      return;
   }

   new ( renderer() ) RCBindPixelShader( *m_selectionMarker );
   m_geometry.render( renderer );
   new ( renderer() ) RCUnbindPixelShader( *m_selectionMarker );
}

///////////////////////////////////////////////////////////////////////////////
