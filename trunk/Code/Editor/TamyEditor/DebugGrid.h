/// @file   TamyEditor/DebugGrid.h
/// @brief  debug grid
#pragma once

#include "core-Renderer/Geometry.h"


///////////////////////////////////////////////////////////////////////////////

class SingleColorDebugMat;
class LineSegments;

///////////////////////////////////////////////////////////////////////////////

/**
 * Debug grid entity.
 */
class DebugGrid : public Geometry
{
   DECLARE_CLASS()

private:
   SingleColorDebugMat*    m_material;
   LineSegments*           m_mesh;

public:
   /**
    * Constructor.
    */
   DebugGrid();
   ~DebugGrid();

   // -------------------------------------------------------------------------
   // Geometry implementation
   // -------------------------------------------------------------------------
   bool onPreRender( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
