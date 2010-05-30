#pragma once

/// @file   TamyEditor/SelectedRenderableJoint.h
/// @brief  A visual representation of a selected renderable joint

#include "SelectedEntityRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

class RenderableJoint;
class Gizmo;

///////////////////////////////////////////////////////////////////////////////

/**
 * A visual representation of a selected renderable joint.
 */
class SelectedRenderableJoint : public SelectedEntityRepresentation
{
private:
   RenderableJoint&           m_joint;
   Gizmo*                     m_gizmo;
   Attributes                 m_attributes;

public:
   /**
    * Constructor.
    *
    * @param joint
    */
   SelectedRenderableJoint( RenderableJoint& joint );

   // -------------------------------------------------------------------------
   // Attributed implementation
   // -------------------------------------------------------------------------
   const Attributes& getAttributes() const;
   void render();

   // -------------------------------------------------------------------------
   // SelectedEntityRepresentation implementation
   // -------------------------------------------------------------------------
   void initialize( SelectionManager& host );
};

///////////////////////////////////////////////////////////////////////////////
