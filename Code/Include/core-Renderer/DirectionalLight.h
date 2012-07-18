/// @file   core-Renderer\DirectionalLight.h
/// @brief  a directional light
#ifndef _DIRECTIONAL_LIGHT_H
#define _DIRECTIONAL_LIGHT_H

#include "core-Renderer\Light.h"
#include "core-Renderer\RenderCommand.h"
#include "core\Color.h"
#include "core\UniqueObject.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a special kind of node that represents a directional light lighting the scene
 */
class DirectionalLight : public Light, public UniqueObject< DirectionalLight >
{
   DECLARE_CLASS();

public:
   Color m_ambient;
   Color m_diffuse;
   Color m_specular;

public:
   DirectionalLight( const std::string& name = "" );
   virtual ~DirectionalLight() {}

   // -------------------------------------------------------------------------
   // Light implementation
   // -------------------------------------------------------------------------
   void render( Renderer& renderer );

protected:
   void onAccept( NodeVisitor& visitor );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A command that renders a single directional light.
 */
class RCRenderDirectionalLight : public RenderCommand
{
private:
   DirectionalLight&       m_light;

public:
   /**
    * Constructor.
    *
    * @param light
    */
   RCRenderDirectionalLight( DirectionalLight& light ) : m_light( light ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

#endif // _DIRECTIONAL_LIGHT_H
