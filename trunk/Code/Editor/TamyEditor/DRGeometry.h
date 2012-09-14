/// @file   TamyEditor/DRGeometry.h
/// @brief  debug representation of a Geometry entity
#pragma once

#include "DebugGeometry.h"


///////////////////////////////////////////////////////////////////////////////

class Geometry;
class TriangleMesh;
class SingleColorDebugMat;

///////////////////////////////////////////////////////////////////////////////

class DRGeometry : public DebugGeometry
{
   DECLARE_CLASS()

private:
   Geometry&               m_geometry;

   TriangleMesh*           m_mesh;
   SingleColorDebugMat*    m_material;

public:
   /**
    * Constructor.
    *
    * @param light
    */
   DRGeometry( Geometry& geometry );
   ~DRGeometry();

   // -------------------------------------------------------------------------
   // DebugGeometry implementation
   // -------------------------------------------------------------------------
   void enableBoundingBox( bool enable );
   void enableDebugShape( bool enable );
};

///////////////////////////////////////////////////////////////////////////////
