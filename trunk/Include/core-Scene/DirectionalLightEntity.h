#pragma once

/// @file   core-Scene\DirectionalLightEntity.h
/// @brief  a directional light description

#include "core-Scene\Entity.h"
#include "core\Color.h"
#include <d3dx9.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes a directional light.
 */
struct DirectionalLightEntity : public Entity
{
   std::string m_name;
   Color m_ambient;
   Color m_diffuse;
   Color m_specular;
   D3DXMATRIX m_situation;

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
   // Entity implementation
   // -------------------------------------------------------------------------
   void save(Serializer& serializer);

   void load(Serializer& serializer);
};

///////////////////////////////////////////////////////////////////////////////
