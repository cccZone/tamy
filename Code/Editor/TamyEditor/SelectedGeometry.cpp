#include "SelectedGeometry.h"
#include "core-Renderer.h"
#include "SelectionManager.h"
#include "tamyeditor.h"


///////////////////////////////////////////////////////////////////////////////

SelectedGeometry::SelectedGeometry( Geometry& geometry )
: m_geometry( geometry )
, m_selectionMarker( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

SelectedGeometry::~SelectedGeometry()
{
   m_selectionMarker = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SelectedGeometry::initialize( SelectionManager& host )
{
   TamyEditor& servicesMgr = host.getServicesMgr();

   // acquire an instance of the resources manager and a camera
   ResourcesManager& resMgr = servicesMgr.requestService< ResourcesManager >();
   Camera& camera = servicesMgr.requestService< Camera >();

   // create a selection marker effect
   static const char* shaderName = "Editor/Shaders/SelectionMarker.psh";
   m_selectionMarker = dynamic_cast< PixelShader* >( resMgr.findResource( "SelectionMarker" ) );
   if ( !m_selectionMarker )
   {
      m_selectionMarker = new PixelShader( "SelectionMarker" );
      m_selectionMarker->loadFromFile( resMgr.getFilesystem(), shaderName );
      resMgr.addResource( m_selectionMarker );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SelectedGeometry::render()
{
   if ( !m_selectionMarker || !m_geometry.isVisible() || !m_geometry.hasGeometry() )
   {
      return;
   }

   m_selectionMarker->beginRendering();
   m_geometry.render();
   m_selectionMarker->endRendering();
}

///////////////////////////////////////////////////////////////////////////////

const Attributes& SelectedGeometry::getAttributes() const
{
   return m_attributes;
}

///////////////////////////////////////////////////////////////////////////////
