/// @file   core-Renderer\DirectionalLight.h
/// @brief  a directional light
#ifndef _DIRECTIONAL_LIGHT_H
#define _DIRECTIONAL_LIGHT_H

#include "core-Renderer\Light.h"
#include "core\Color.h"
#include "core\UniqueObject.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class Geometry;
class RenderTarget;
class RenderingView;
class Camera;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a special kind of node that represents a directional light in a scene
 */
class DirectionalLight : public Light, public UniqueObject< DirectionalLight >
{
   DECLARE_CLASS();

public:
   // static data
   Color                m_color;
   float                m_strength;

public:
   /**
    * Constructor.
    *
    * @param name
    */
   DirectionalLight( const std::string& name = "" );

   /**
    * Copy constructor.
    *
    * @param rhs
    */
   DirectionalLight( const DirectionalLight& rhs );

protected:
   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   Entity* cloneSelf() const;
};

///////////////////////////////////////////////////////////////////////////////

#endif // _DIRECTIONAL_LIGHT_H
