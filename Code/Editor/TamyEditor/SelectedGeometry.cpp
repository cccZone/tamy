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
   static const char* shaderName = "Editor/Shaders/SelectionMarker.psh";
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   m_selectionMarker = dynamic_cast< PixelShader* >( resMgr.findResource( "SelectionMarker" ) );
   if ( !m_selectionMarker )
   {
      m_selectionMarker = new PixelShader( "SelectionMarker" );
      m_selectionMarker->loadFromFile( resMgr.getFilesystem(), shaderName );
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
