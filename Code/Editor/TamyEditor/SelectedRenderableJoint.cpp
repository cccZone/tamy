#include "SelectedRenderableJoint.h"
#include "core-Renderer\RenderableJoint.h"
#include "SelectionManager.h"
#include "Gizmo.h"


///////////////////////////////////////////////////////////////////////////////

SelectedRenderableJoint::SelectedRenderableJoint( RenderableJoint& joint )
: m_joint( joint )
, m_gizmo( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void SelectedRenderableJoint::initialize( SelectionManager& host )
{
   // get the gizmo the manager is using that visualizes the objects
   // manipulation mode
   m_gizmo = &host.getGizmo();
}

///////////////////////////////////////////////////////////////////////////////

void SelectedRenderableJoint::render()
{
   if ( m_gizmo )
   {
      m_gizmo->render( m_joint.getGlobalMtx() );
   }
}

///////////////////////////////////////////////////////////////////////////////

const Attributed::Attributes& SelectedRenderableJoint::getAttributes() const
{
   return m_attributes;
}

///////////////////////////////////////////////////////////////////////////////
