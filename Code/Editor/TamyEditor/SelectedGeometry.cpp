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
   static FilePath shaderName( "Editor/Shaders/SelectionMarker.tpsh" );
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   m_selectionMarker = resMgr.create< PixelShader >( shaderName );
}

///////////////////////////////////////////////////////////////////////////////

SelectedGeometry::~SelectedGeometry()
{
   m_selectionMarker = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SelectedGeometry::render( Renderer& renderer )
{
   if ( !m_geometry.hasMesh() )
   {
      return;
   }

   new ( renderer() ) RCBindPixelShader( *m_selectionMarker, renderer );
   new ( renderer() ) RCSetFillMode( GFM_Wireframe );
   m_geometry.render( renderer );
   new ( renderer() ) RCSetFillMode( GFM_Solid );
   new ( renderer() ) RCUnbindPixelShader( *m_selectionMarker, renderer );
}

///////////////////////////////////////////////////////////////////////////////
