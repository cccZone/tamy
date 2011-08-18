#pragma once

/// @file   TamyEditor/SelectedGeometry.h
/// @brief  A visual representation of a selected renderable

#include "SelectedEntityRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

class Geometry;
class PixelShader;

///////////////////////////////////////////////////////////////////////////////

/**
 * A visual representation of a selected geometry.
 */
class SelectedGeometry : public SelectedEntityRepresentation
{
private:
   Geometry&                  m_geometry;
   PixelShader*               m_selectionMarker;

public:
   /**
    * Constructor.
    *
    * @param geometry
    */
   SelectedGeometry( Geometry& geometry );
   ~SelectedGeometry();

   // -------------------------------------------------------------------------
   // SelectedEntityRepresentation implementation
   // -------------------------------------------------------------------------
   void render( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
