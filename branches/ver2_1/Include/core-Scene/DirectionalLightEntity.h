#pragma once

/// @file   core-Scene\DirectionalLightEntity.h
/// @brief  a directional light description

#include "core-Scene\Situated.h"
#include "core-Scene\Entity.h"
#include "core\Color.h"
#include <d3dx9.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes a directional light.
 */
struct DirectionalLightEntity : public Entity, public Situated
{
public:
   std::string m_name;
   Color m_ambient;
   Color m_diffuse;
   Color m_specular;
   D3DXMATRIX m_situation;

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
   // Situated implementation
   // -------------------------------------------------------------------------
   const D3DXMATRIX& getSituation() const {return m_situation;}

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
