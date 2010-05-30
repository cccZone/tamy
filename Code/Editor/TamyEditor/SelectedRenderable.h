#pragma once

/// @file   TamyEditor/SelectedRenderable.h
/// @brief  A visual representation of a selected renderable

#include "SelectedEntityRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

class Renderable;
class Effect;
class Gizmo;

///////////////////////////////////////////////////////////////////////////////

/**
 * A visual representation of a selected renderable.
 */
class SelectedRenderable : public SelectedEntityRepresentation
{
private:
   Renderable&                m_renderable;
   Effect*                    m_selectionMarker;
   Gizmo*                     m_gizmo;
   Attributes                 m_attributes;

public:
   /**
    * Constructor.
    *
    * @param renderable
    */
   SelectedRenderable( Renderable& renderable );
   ~SelectedRenderable();

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
