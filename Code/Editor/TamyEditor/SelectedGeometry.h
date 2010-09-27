#pragma once

/// @file   TamyEditor/SelectedGeometry.h
/// @brief  A visual representation of a selected renderable

#include "SelectedEntityRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

class Geometry;
class Shader;

///////////////////////////////////////////////////////////////////////////////

/**
 * A visual representation of a selected geometry.
 */
class SelectedGeometry : public SelectedEntityRepresentation
{
private:
   Geometry&                  m_geometry;
   Shader*                    m_selectionMarker;
   Attributes                 m_attributes;

public:
   /**
    * Constructor.
    *
    * @param renderable
    */
   SelectedGeometry( Geometry& geometry );
   ~SelectedGeometry();

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
