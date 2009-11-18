#pragma once

/// @file   core-Scene\CameraEntity.h
/// @brief  a camera description

#include "core-Scene\SpatiallyQueryable.h"
#include "core-Scene\Entity.h"
#include "core\BoundingSphere.h"
#include <d3dx9.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes a directional light.
 */
struct CameraEntity : public Entity, public SpatiallyQueryable
{
public:
   std::string m_name;
   D3DXMATRIX m_situation;

private:
   BoundingSphere m_boundingVol;
   Array<Triangle*> m_noGeometry;

public:
   /**
    * Default constructor.
    */
   CameraEntity();

   /**
    * Constructor.
    *
    * @param name
    * @param situation
    */
   CameraEntity(const std::string& name,
                const D3DXMATRIX& situation);

   // -------------------------------------------------------------------------
   // SpatiallyQueryable implementation
   // -------------------------------------------------------------------------
   const BoundingVolume& getBoundingVolume();

   const D3DXMATRIX& getGlobalMtx();

   const Array<Triangle*>& getBoundingGeometry();

   // -------------------------------------------------------------------------
   // Serializable implementation
   // -------------------------------------------------------------------------
   void save(Serializer& serializer);

   void load(Serializer& serializer);

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
protected:
   void registerProperties();
};

///////////////////////////////////////////////////////////////////////////////
