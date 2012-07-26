/// @file   core-Renderer\DirectionalLight.h
/// @brief  a directional light
#ifndef _DIRECTIONAL_LIGHT_H
#define _DIRECTIONAL_LIGHT_H

#include "core-Renderer\Light.h"
#include "core-Renderer\RenderCommand.h"
#include "core\Color.h"
#include "core\UniqueObject.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a special kind of node that represents a directional light lighting the scene
 */
class DirectionalLight : public Light, public UniqueObject< DirectionalLight >
{
   DECLARE_CLASS();

public:
   Color          m_color;

   PixelShader*   m_pixelShader;

public:
   DirectionalLight( const std::string& name = "" );
   virtual ~DirectionalLight();

   // -------------------------------------------------------------------------
   // Light implementation
   // -------------------------------------------------------------------------
   void render( Renderer& renderer, ShaderTexture* depthNormalsBufferTex );

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

private:
   void initialize();
};

///////////////////////////////////////////////////////////////////////////////


#endif // _DIRECTIONAL_LIGHT_H
