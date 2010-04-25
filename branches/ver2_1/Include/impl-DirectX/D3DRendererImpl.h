#pragma once

#include <d3d9.h>
#include "core-Renderer\RendererImpl.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

enum SourceCode;
class Light;

///////////////////////////////////////////////////////////////////////////////

class D3DRendererImpl : public RendererImpl
{
private:
   struct CachedLight
   {
      D3DLIGHT9 light;
      unsigned int lastChangeIdx;

      CachedLight() : lastChangeIdx(-1) {}
   };

private:
   IDirect3DDevice9& m_d3Device;
   unsigned int m_maxLightsCount;

   D3DMATERIAL9 m_material;

   // optimization used to prevent constant memory copying when lights 
   // are set
   Array<CachedLight>  m_cachedLights;

public:
   /**
    * Constructor.
    *
    * @param d3Device           running Direct3D device on which we want 
    *                           to render
    * @param maxLightsCount     maximum lights that can be rendered on 
    *                           the specified device
    */
   D3DRendererImpl(IDirect3DDevice9& d3Device, unsigned int maxLightsCount);

   void render(Renderable& renderable, RenderingTargetsPolicy& policy);

   void enableLights(const Array<Light*>& lights);

   void disableLights(const Array<Light*>& lights);

   void setMatrices(const D3DXMATRIX& viewMtx, const D3DXMATRIX& projMtx);

   void setObjectMatrices(const Array<D3DXMATRIX>& matrices);

   void setTransparency(bool enable);
   
   void setLRP(LightReflectingProperties& lrp);

   void setTexture(unsigned char stageIndex, Texture& texture);

   void setColorOperation(unsigned char stageIndex, MaterialOperation& colorOp);

   void setAlphaOperation(unsigned char stageIndex, MaterialOperation& alphaOp);

   void setCoordsOperation(unsigned char stageIndex, CoordinatesOperation& coordsOp);

private:
   DWORD translateArg(SourceCode code) const;

   void updateLightInfo(unsigned int lightIdx, Light& lt);

};

///////////////////////////////////////////////////////////////////////////////
