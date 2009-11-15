#pragma once

/// @file   core-Scene\PointLightEntity.h
/// @brief  a point light description

#include "core-Scene\Entity.h"
#include "core\Color.h"
#include <d3dx9.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes a point light.
 */
struct PointLightEntity : public Entity
{
   std::string m_name;
   Color m_ambient;
   Color m_diffuse;
   Color m_specular;
   float m_range;
   float m_constAtt;
   float m_linearAtt;
   float m_quadAtt;
   D3DXVECTOR3 m_situation;

   /**
    * Default constructor.
    */
   PointLightEntity();

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
    * @param situation
    */
   PointLightEntity(const std::string& name,
                    const Color& ambient, 
                    const Color& diffuse, 
                    const Color& specular,
                    float range,
                    float constAtt, float linearAtt, float quadAtt,
                    const D3DXVECTOR3& situation);

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void save(Serializer& serializer);

   void load(Serializer& serializer);
};

///////////////////////////////////////////////////////////////////////////////
