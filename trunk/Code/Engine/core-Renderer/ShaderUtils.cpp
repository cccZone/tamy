#include "core-Renderer\ShaderUtils.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\ShaderTexture.h"
#include "core\Vector.h"


///////////////////////////////////////////////////////////////////////////////

void ShaderUtils::calculateHalfPixel( Renderer& renderer, ShaderTexture* inputTex, Vector& outHalfPixel )
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
