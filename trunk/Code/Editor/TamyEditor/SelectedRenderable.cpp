#include "SelectedRenderable.h"
#include "core-Renderer\Renderable.h"
#include "SelectionManager.h"
#include "SelectionMarker.h"
#include "Gizmo.h"


///////////////////////////////////////////////////////////////////////////////

SelectedRenderable::SelectedRenderable( Renderable& renderable )
: m_renderable( renderable )
, m_selectionMarker( NULL )
, m_gizmo( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

SelectedRenderable::~SelectedRenderable()
{
   delete m_selectionMarker;
   m_selectionMarker = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SelectedRenderable::initialize( SelectionManager& host )
{
   // create a selection marker effect
   m_selectionMarker = new SelectionMarker( host.getResourcesManager(), host.getCamera(), m_renderable );

   // get the gizmo the manager is using that visualizes the objects
   // manipulation mode
   m_gizmo = &host.getGizmo();
}

///////////////////////////////////////////////////////////////////////////////

void SelectedRenderable::render()
{
   if ( !m_selectionMarker || !m_renderable.isVisible() || !m_renderable.hasGeometry() )
   {
      return;
   }

   m_selectionMarker->render( m_renderable.getGeometry() );

   if ( m_gizmo )
   {
      m_gizmo->render( m_renderable.getGlobalMtx() );
   }
}

///////////////////////////////////////////////////////////////////////////////

const Attributed::Attributes& SelectedRenderable::getAttributes() const
{
   return m_attributes;
}

///////////////////////////////////////////////////////////////////////////////
