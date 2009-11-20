#pragma once

/// @file   core-Scene\StaticGeometryEntity.h
/// @brief  description of scenery made up of static geometry

#include "core-Scene\SpatiallyQueryable.h"
#include "core-Scene\Entity.h"
#include "core-Scene\MeshDesc.h"
#include "core\AABoundingBox.h"
#include <vector>
#include <d3dx9.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes a scenery made up of static geometry.
 */
struct StaticGeometryEntity : public Entity, public SpatiallyQueryable
{
public:
   std::vector<MeshDesc*> m_meshes;
   D3DXMATRIX m_situation;

private:
   AABoundingBox m_localBoundingVol;
   AABoundingBox m_globalBoundingVol;
   Array<Triangle*> m_geometry;

public:
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
   // SpatiallyQueryable implementation
   // -------------------------------------------------------------------------
   const BoundingVolume& getBoundingVolume();

   const D3DXMATRIX& getGlobalMtx();

   const Array<Triangle*>& getBoundingGeometry();

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void save(Serializer& serializer);

   void load(Serializer& serializer);

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
protected:
   void registerProperties();

private:
   void resetGeometry();
   void calculateGeometry();
};

///////////////////////////////////////////////////////////////////////////////
