/// @file   core-Renderer/AmbientLight.h
/// @brief  a light that provides a uniform, ambient lighting for the scene
#pragma once

#include "core-Renderer\Light.h"
#include "core\Color.h"
#include "core\UniqueObject.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A light that provides a uniform, ambient lighting for the scene.
 */
class AmbientLight : public Light, public UniqueObject< AmbientLight >
{
   DECLARE_CLASS();

public:
   /**
    * Constructor.
    */
   AmbientLight( const std::string& name = "" );
   ~AmbientLight();

   // -------------------------------------------------------------------------
   // Light implementation
   // -------------------------------------------------------------------------
   void render( Renderer& renderer, const LightingRenderData& data );

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
};

///////////////////////////////////////////////////////////////////////////////
