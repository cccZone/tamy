#include "SelectionRenderingPass.h"
#include "core-Renderer.h"
#include "SelectedEntityRepresentation.h"

// representations
#include "SelectedGeometry.h"

///////////////////////////////////////////////////////////////////////////////

SelectionRenderingPass::SelectionRenderingPass( SelectionManager& host )
: RenderingPass( NULL )
, m_host( host )
, m_selectedRepresentation( NULL )
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

void SelectionRenderingPass::set( Entity* entity )
{
   // remove the previous representation
   if ( m_selectedRepresentation )
   {
      statesMgr().remove( *m_selectedRepresentation );
      delete m_selectedRepresentation;
      m_selectedRepresentation = NULL;
   }

   if ( entity )
   {
      m_selectedRepresentation = create( *entity );
      if ( m_selectedRepresentation )
      {
         m_selectedRepresentation->initialize( m_host );
         statesMgr().add( *m_selectedRepresentation );
      }
   }

}

///////////////////////////////////////////////////////////////////////////////
