/// @file   TamyEditor/DRPointLight.h
/// @brief  debug representation of a point light
#pragma once

#include "DebugGeometry.h"


///////////////////////////////////////////////////////////////////////////////

class PointLight;
class Geometry;
class TriangleMesh;
class SingleColorDebugMat;

///////////////////////////////////////////////////////////////////////////////

class DRPointLight : public DebugGeometry
{
   DECLARE_CLASS()

private:
   PointLight&             m_light;

   TriangleMesh*           m_mesh;
   SingleColorDebugMat*    m_material;

public:
   /**
    * Constructor.
    *
    * @param light
    */
   DRPointLight( PointLight& light );
   ~DRPointLight();
};

///////////////////////////////////////////////////////////////////////////////
