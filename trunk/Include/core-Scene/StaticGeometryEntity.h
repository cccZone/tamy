#pragma once

/// @file   core-Scene\StaticGeometryEntity.h
/// @brief  description of scenery made up of static geometry

#include "core-Scene\Entity.h"
#include "core-Scene\MeshDesc.h"
#include <vector>
#include <d3dx9.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes a scenery made up of static geometry.
 */
struct StaticGeometryEntity : public Entity
{
   std::vector<MeshDesc*> m_meshes;
   D3DXMATRIX m_situation;

   /**
    * Default constructor.
    */
   StaticGeometryEntity();

   /**
    * Constructor.
    *
    * @param mesh
    * @param materials
    * @param situation
    */
   StaticGeometryEntity(const std::vector<MeshDesc*> meshes,
                        const D3DXMATRIX& situation);
   ~StaticGeometryEntity();

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void save(Serializer& serializer);

   void load(Serializer& serializer);
};

///////////////////////////////////////////////////////////////////////////////
