#pragma once

/// @file   core-Scene\SpotLightEntity.h
/// @brief  a spot light description

#include "core-Scene\SpatiallyQueryable.h"
#include "core-Scene\Entity.h"
#include "core\Color.h"
#include "core\BoundingSphere.h"
#include <d3dx9.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes a spot light.
 */
struct SpotLightEntity : public Entity, public SpatiallyQueryable
{
public:
   std::string m_name;
   Color m_ambient;
   Color m_diffuse;
   Color m_specular;
   float m_range;
   float m_constAtt;
   float m_linearAtt;
   float m_quadAtt;
   float m_fallOff;
   float m_theta;
   float m_phi;
   D3DXMATRIX m_situation;

private:
   BoundingSphere m_boundingVol;
   Array<Triangle*> m_noGeometry;

public:
   /**
    * Default constructor.
    */
   SpotLightEntity();

   /**
    * Constructor.
    *
    * @param name
    * @param ambient
    * @param diffuse
    * @param specular
    * @param range
    * @param constAtt
    * @param linearAtt
    * @param quadAtt
    * @param fallOff
    * @param theta
    * @param phi
    * @param situation
    */
   SpotLightEntity(const std::string& name,
                   const Color& ambient, 
                   const Color& diffuse, 
                   const Color& specular,
                   float range,
                   float constAtt, float linearAtt, float quadAtt,
                   float fallOff, float theta, float phi,
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
