#include "SelectionRenderingPass.h"
#include "core-Renderer.h"
#include "SelectedEntityRepresentation.h"

// representations
#include "SelectedGeometry.h"


///////////////////////////////////////////////////////////////////////////////

SelectionRenderingPass::SelectionRenderingPass()
   : m_selectedRepresentation( NULL )
{
   // define associations
   associateAbstract< Geometry, SelectedGeometry >();
}

///////////////////////////////////////////////////////////////////////////////

SelectionRenderingPass::~SelectionRenderingPass()
{
   delete m_selectedRepresentation;
   m_selectedRepresentation = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SelectionRenderingPass::add( Entity& entity )
{
   // remove the previous representation
   if ( m_selectedRepresentation )
   {
      delete m_selectedRepresentation;
      m_selectedRepresentation = NULL;
   }

   m_selectedRepresentation = create( entity );
}

///////////////////////////////////////////////////////////////////////////////

void SelectionRenderingPass::remove( Entity& entity )
{
   if ( m_selectedRepresentation )
   {
      delete m_selectedRepresentation;
      m_selectedRepresentation = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void SelectionRenderingPass::initialize( Renderer& renderer )
{

}

///////////////////////////////////////////////////////////////////////////////

void SelectionRenderingPass::deinitialize( Renderer& renderer )
{

}

///////////////////////////////////////////////////////////////////////////////

void SelectionRenderingPass::render( Renderer& renderer )
{
   new ( renderer() ) RCActivateRenderTarget( NULL );

   if ( m_selectedRepresentation )
   {
      m_selectedRepresentation->render( renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////
