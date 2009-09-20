#pragma once

/// @file   core-ResoureManagement\StaticGeometry.h
/// @brief  output in which StaticGeometryLoader stores the meshes it reads

#include "core-Renderer\LitVertex.h"
#include "core-Renderer\Face.h"
#include "core-Renderer\Material.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * StaticGeometryLoader will use this interface to store meshes
 * it loads.
 */
class StaticGeometry
{
public:
   virtual ~StaticGeometry() {}

   virtual void addMesh(const std::vector<LitVertex>& vertices,
                        const std::vector<Face<USHORT> >& faces,
                        Material& material) = 0;
};

///////////////////////////////////////////////////////////////////////////////
