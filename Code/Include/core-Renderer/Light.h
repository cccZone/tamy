/// @file   core-Renderer/Light.h
/// @brief  base class and render commands for lights
#pragma once

#include "core-MVC/SpatialEntity.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Base class for the light.
 */
class Light : public SpatialEntity
{
   DECLARE_CLASS();

public:
   enum LightType
   {
      LT_Directional,
      LT_Point
   };

protected:
   bool           m_castsShadows;
   LightType      m_lightType;

public:
   /**
    * Constructor.
    *
    * @param type
    * @param name
    */
   Light( LightType type, const std::string& name = "" );
   virtual ~Light() {}

   /**
    * Returns type of this light.
    */
   inline LightType getType() const { return m_lightType; }

   /**
    * Enables or disables the shadows cast by this light.
    *
    * @param enable
    */
   inline void setShadowsCaster( bool enable ) { m_castsShadows = enable; }

   /**
    * Tells if the light is set to cast shadows.
    */
   inline bool castsShadows() const { return m_castsShadows; }
};

///////////////////////////////////////////////////////////////////////////////
