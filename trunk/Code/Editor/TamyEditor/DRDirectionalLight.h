/// @file   TamyEditor/DRDirectionalLight.h
/// @brief  debug representation of a directional light
#pragma once

#include "DebugGeometry.h"


///////////////////////////////////////////////////////////////////////////////

class DirectionalLight;
class Geometry;
class TriangleMesh;
class SingleColorDebugMat;

///////////////////////////////////////////////////////////////////////////////

class DRDirectionalLight : public DebugGeometry
{
   DECLARE_CLASS()

private:
   DirectionalLight&       m_light;

   TriangleMesh*           m_mesh;
   SingleColorDebugMat*    m_material;

public:
   /**
    * Constructor.
    *
    * @param light
    */
   DRDirectionalLight( DirectionalLight& light );
   ~DRDirectionalLight();

   // -------------------------------------------------------------------------
   // DebugGeometry implementation
   // -------------------------------------------------------------------------
   void enableBoundingBox( bool enable );
   void enableDebugShape( bool enable );
};

///////////////////////////////////////////////////////////////////////////////
