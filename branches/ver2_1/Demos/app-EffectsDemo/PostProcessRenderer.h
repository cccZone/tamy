#pragma once

#include "core-Renderer\RenderingEffectRenderer.h"
#include "core-Renderer\GraphicalEffect.h"


///////////////////////////////////////////////////////////////////////////////

class PostProcessRenderer : public RenderingEffectRenderer
{
public:
   PostProcessRenderer(GraphicalEffect* effect)
      : RenderingEffectRenderer(effect)
   {}

   void onRender()
   {
      effect().setTechnique("render");
   }

   void enableLights(const Array<Light*>& lights)
   {}

   void disableLights(const Array<Light*>& lights)
   {}

   void setObjectMatrices(const Array<D3DXMATRIX>& matrices)
   {}
   
   void setMatrices(const D3DXMATRIX& viewMtx, const D3DXMATRIX& projMtx)
   {}

   void setTransparency(bool enable) 
   {}
   
   void setLRP(LightReflectingProperties& lrp) 
   {}

   void setTexture(unsigned char stageIndex, Texture& texture)
   {
      effect().setTexture("inputTexture", texture);
   }

   void setColorOperation(unsigned char stageIndex, 
                          MaterialOperation& colorOp)
   {}

   void setAlphaOperation(unsigned char stageIndex, 
                          MaterialOperation& alphaOp)
   {}

   void setCoordsOperation(unsigned char stageIndex, 
                           CoordinatesOperation& coordsOp)
   {}

};

///////////////////////////////////////////////////////////////////////////////
