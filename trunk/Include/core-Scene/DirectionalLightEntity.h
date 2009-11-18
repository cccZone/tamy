#pragma once

/// @file   core-Scene\DirectionalLightEntity.h
/// @brief  a directional light description

#include "core-Scene\SpatiallyQueryable.h"
#include "core-Scene\Entity.h"
#include "core\Color.h"
#include "core\BoundingSphere.h"
#include <d3dx9.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes a directional light.
 */
struct DirectionalLightEntity : public Entity, public SpatiallyQueryable
{
public:
   std::string m_name;
   Color m_ambient;
   Color m_diffuse;
   Color m_specular;
   D3DXMATRIX m_situation;

private:
   BoundingSphere m_boundingVol;
   Array<Triangle*> m_noGeometry;

public:
   /**
    * Default constructor.
    */
   DirectionalLightEntity();

   /**
    * Constructor.
    *
    * @param name
    * @param ambient
    * @param diffuse
    * @param specular
    * @param situation
    */
   DirectionalLightEntity(const std::string& name,
                          const Color& ambient, 
                          const Color& diffuse, 
                          const Color& specular,
                          const D3DXMATRIX& situation);

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
};

///////////////////////////////////////////////////////////////////////////////
