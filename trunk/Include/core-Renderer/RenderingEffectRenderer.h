#pragma once

#include "core-Renderer\RendererImpl.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class GraphicalEffect;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class as an adapter between what we expect from the RendererImpl,
 * and how can we achieve that using a GraphicalEffect
 */
class RenderingEffectRenderer : public RendererImpl
{
private:
   GraphicalEffect* m_effect;

public:
   virtual ~RenderingEffectRenderer();

   void render(Renderable& renderable, RenderingTargetsPolicy& policy);

   /**
    * @inherited
    */
   virtual void enableLights(const Array<Light*>& lights) = 0;

   /**
    * @inherited
    */
   virtual void disableLights(const Array<Light*>& lights) = 0;

   /**
    * @inherited
    */
   virtual void setObjectMatrices(const Array<D3DXMATRIX>& matrices) = 0;
   
   /**
    * @inherited
    */
   virtual void setMatrices(const D3DXMATRIX& viewMtx, const D3DXMATRIX& projMtx) = 0;

   /**
    * @inherited
    */
   virtual void setTransparency(bool enable) = 0;
   
   /**
    * @inherited
    */
   virtual void setLRP(LightReflectingProperties& lrp) = 0;

   /**
    * @inherited
    */
   virtual void setTexture(unsigned char stageIndex, Texture& texture) = 0;

   /**
    * @inherited
    */
   virtual void setColorOperation(unsigned char stageIndex, MaterialOperation& colorOp) = 0;

   /**
    * @inherited
    */
   virtual void setAlphaOperation(unsigned char stageIndex, MaterialOperation& alphaOp) = 0;

   /**
    * @inherited
    */
   virtual void setCoordsOperation(unsigned char stageIndex, CoordinatesOperation& coordsOp) = 0;

protected:
   RenderingEffectRenderer(GraphicalEffect* effect);

   /**
    * This method allows the implementation to access the effect
    * the properties should be set on
    */
   inline GraphicalEffect& effect() {return *m_effect;}

   /**
    * This method is executed just before the effect is rendered,
    * thus allowing the implementation to perform some additional steps
    */
   virtual void onRender() = 0;
};

///////////////////////////////////////////////////////////////////////////////
