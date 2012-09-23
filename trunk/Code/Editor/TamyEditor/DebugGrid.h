/// @file   TamyEditor/DebugGrid.h
/// @brief  debug grid
#pragma once

#include "DebugGeometry.h"


///////////////////////////////////////////////////////////////////////////////

class SingleColorDebugMat;
class LineSegments;
class VertexShader;

///////////////////////////////////////////////////////////////////////////////

/**
 * Debug grid entity.
 */
class DebugGrid : public DebugGeometry
{
   DECLARE_CLASS()

private:
   SingleColorDebugMat*    m_material;
   LineSegments*           m_mesh;
   VertexShader*           m_vertexShader;

public:
   /**
    * Constructor.
    */
   DebugGrid();
   ~DebugGrid();

   /**
    * Shows/hides the grid.
    *
    * @param enable
    */
   void show( bool enable );

   // -------------------------------------------------------------------------
   // Geometry implementation
   // -------------------------------------------------------------------------
   RCBindVertexShader* onPreRender( Renderer& renderer );
   void onPostRender( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
