#pragma once

#include "core-Renderer\RenderingEffectRenderer.h"
#include "core-Renderer\GraphicalEffect.h"
#include "core-Renderer\LightReflectingProperties.h"


///////////////////////////////////////////////////////////////////////////////

class RenderEffectController : public RenderingEffectRenderer
{
private:
   float m_time;
   D3DXMATRIX m_objectWorldMtx;
   D3DXMATRIX m_viewMtx;
   D3DXMATRIX m_projMtx;

public:
   RenderEffectController(GraphicalEffect* effect) 
      : RenderingEffectRenderer(effect)
      , m_time(0) 
   {
      D3DXMatrixIdentity(&m_viewMtx);
      D3DXMatrixIdentity(&m_projMtx);
   }

   void onRender()
   {
      D3DXMATRIX worldView = m_objectWorldMtx * m_viewMtx;
      effect().setMtx("g_mWorldView", worldView);
      effect().setMtx("g_mProjection", m_projMtx);
   }

   void enableLights(const Array<Light*>& lights)
   {
      // TODO: dopisac obsluge swiatel do tego efektu
   }

   void disableLights(const Array<Light*>& lights)
   {
      // do nothing
   }

   void setObjectMatrices(const Array<D3DXMATRIX>& matrices)
   {
      m_objectWorldMtx = matrices[0];
   }

   void setMatrices(const D3DXMATRIX& viewMtx, const D3DXMATRIX& projMtx)
   {
      m_viewMtx = viewMtx;
      m_projMtx = projMtx;
   }

   void setTransparency(bool enable)
   {
   }
   
   void setLRP(LightReflectingProperties& lrp)
   {
      effect().setVec4("g_MaterialAmbientColor", (const D3DXVECTOR4&)lrp.getAmbientColor());
      effect().setVec4("g_MaterialDiffuseColor", (const D3DXVECTOR4&)lrp.getDiffuseColor());
   }

   void setTexture(unsigned char stageIndex, Texture& texture)
   {
      effect().setTexture("g_MeshTexture", texture);
   }

   void setColorOperation(unsigned char stageIndex, MaterialOperation& colorOp)
   {
   }

   void setAlphaOperation(unsigned char stageIndex, MaterialOperation& alphaOp)
   {
   }

   void setCoordsOperation(unsigned char stageIndex, CoordinatesOperation& coordsOp)
   {
   }
};

///////////////////////////////////////////////////////////////////////////////
