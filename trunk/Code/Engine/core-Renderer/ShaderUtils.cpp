#include "core-Renderer\ShaderUtils.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\ShaderTexture.h"
#include "core\Vector.h"


///////////////////////////////////////////////////////////////////////////////

void ShaderUtils::calculateHalfPixel( Renderer& renderer, ShaderTexture* inputTex, Vector& outHalfPixel )
{
   if ( inputTex )
   {
      outHalfPixel[0] = 1.0f / (float)inputTex->getWidth();
      outHalfPixel[1] = 1.0f / (float)inputTex->getHeight();
   }
   else
   {
      outHalfPixel[0] = 1.0f / (float)renderer.getViewportWidth();
      outHalfPixel[1] = 1.0f / (float)renderer.getViewportHeight();
   }
   outHalfPixel[2] = 0.0f;
   outHalfPixel.mul( Float_Inv2 );
}

///////////////////////////////////////////////////////////////////////////////
