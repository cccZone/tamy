#pragma once

/// @file   core-Scene\StaticGeometryEntity.h
/// @brief  description of scenery made up of static geometry

#include "core-Scene\Situated.h"
#include "core-Scene\Spatial.h"
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
struct StaticGeometryEntity : public Entity, 
                              public Situated, 
                              public Spatial
{
public:
   std::vector<MeshDesc*> m_meshes;
   std::string m_name;
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
    * @param name
    * @param meshes
    * @param situation
    */
   StaticGeometryEntity(const std::string& name,
                        const std::vector<MeshDesc*> meshes,
                        const D3DXMATRIX& situation);
   ~StaticGeometryEntity();

   // -------------------------------------------------------------------------
   // Situated implementation
   // -------------------------------------------------------------------------
   const D3DXMATRIX& getSituation() const;

   // -------------------------------------------------------------------------
   // Spatial implementation
   // -------------------------------------------------------------------------
   const BoundingVolume& getBoundingVolume();

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
