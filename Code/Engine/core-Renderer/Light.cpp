#include "core-Renderer/Light.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( Light );
   PARENT( SpatialEntity );
   PROPERTY_EDIT( "Casts shadows", bool, m_castsShadows );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

Light::Light( const std::string& name )
   : SpatialEntity( name )
   , m_castsShadows( false )
{
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer/Renderer.h"
#include "core-Renderer/ShaderTexture.h"

void LightUtils::calculateHalfPixel( Renderer& renderer, ShaderTexture* inputTex, Vector& outHalfPixel )
{
   if ( inputTex )
   {
      outHalfPixel.x = 1.0f / (float)inputTex->getWidth();
      outHalfPixel.y = 1.0f / (float)inputTex->getHeight();
   }
   else
   {
      outHalfPixel.x = 1.0f / (float)renderer.getViewportWidth();
      outHalfPixel.y = 1.0f / (float)renderer.getViewportHeight();
   }
   outHalfPixel.z = 0.0f;
   outHalfPixel.mul( 0.5f );
}

///////////////////////////////////////////////////////////////////////////////
