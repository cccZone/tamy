/// @file   core-Renderer/PointLight.h
/// @brief  point light entity
#pragma once

#include "core-Renderer\Light.h"
#include "core-Renderer\RenderCommand.h"
#include "core\Color.h"
#include "core\UniqueObject.h"


///////////////////////////////////////////////////////////////////////////////

struct BoundingSphere;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a special kind of node that represents a point light in a scene
 */
class PointLight : public Light, public UniqueObject< PointLight >
{
   DECLARE_CLASS();

public:
   // static data
   Color             m_color;
   float             m_strength;
   float             m_attenuation;
   float             m_radius;

   // runtime data
   BoundingSphere*   m_boundingSphere;

public:
   /**
    * Constructor.
    *
    * @param name
    */
   PointLight( const std::string& name = "" );

   /**
    * Copy constructor.
    *
    * @param rhs
    */
   PointLight( const PointLight& rhs );
   virtual ~PointLight();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
   void onPropertyChanged( ReflectionProperty& property );

protected:
   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   Entity* cloneSelf() const;

private:
   void initialize();
};

///////////////////////////////////////////////////////////////////////////////
