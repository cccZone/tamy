#include "SelectedRenderable.h"
#include "core-Renderer\Renderable.h"
#include "SelectionManager.h"
#include "SelectionMarker.h"
#include "tamyeditor.h"


///////////////////////////////////////////////////////////////////////////////

SelectedRenderable::SelectedRenderable( Renderable& renderable )
: m_renderable( renderable )
, m_selectionMarker( NULL )
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
   TamyEditor& servicesMgr = host.getServicesMgr();

   // acquire an instance of the resources manager and a camera
   ResourcesManager& resMgr = servicesMgr.requestService< ResourcesManager >();
   Camera& camera = servicesMgr.requestService< Camera >();

   // create a selection marker effect
   m_selectionMarker = new SelectionMarker( resMgr, camera, m_renderable );
}

///////////////////////////////////////////////////////////////////////////////

void SelectedRenderable::render()
{
   if ( !m_selectionMarker || !m_renderable.isVisible() || !m_renderable.hasGeometry() )
   {
      return;
   }

   m_selectionMarker->render( m_renderable.getGeometry() );
}

///////////////////////////////////////////////////////////////////////////////

const Attributed::Attributes& SelectedRenderable::getAttributes() const
{
   return m_attributes;
}

///////////////////////////////////////////////////////////////////////////////
