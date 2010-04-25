#include "impl-DirectX\D3DRendererImpl.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"
#include "core-Renderer\RenderingTargetsPolicy.h"
#include "core-Renderer\Renderable.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

D3DRendererImpl::D3DRendererImpl(IDirect3DDevice9& d3Device, 
                                 unsigned int maxLightsCount)
: m_d3Device(d3Device)
, m_maxLightsCount(maxLightsCount)
, m_cachedLights(4098)  // requires some initial memory, but this way we'll prevent fragmentation
{
}

///////////////////////////////////////////////////////////////////////////////

void D3DRendererImpl::render(Renderable& renderable, RenderingTargetsPolicy& policy)
{
   policy.setTargets(0);
   renderable.render();
}

///////////////////////////////////////////////////////////////////////////////

void D3DRendererImpl::enableLights(const Array<Light*>& lights)
{
   unsigned int lightsCount = min(lights.size(), m_maxLightsCount);
   for (unsigned int i = 0; i < lightsCount; ++i)
   {
      Light& lt = *(lights[i]);
      unsigned ltIdx = lt.getIndex();

      if (ltIdx >= m_cachedLights.size())
      {
         m_cachedLights.allocate(ltIdx + 1);
         m_cachedLights.push_back(CachedLight());
         updateLightInfo(ltIdx, lt);
      }
      else if (lt.getChangeIdx() != m_cachedLights[ltIdx].lastChangeIdx)
      {
         updateLightInfo(ltIdx, lt);
      }
      
      // unfortunately we need to update light position and orientation every time
      D3DLIGHT9 d3dLight = m_cachedLights[ltIdx].light;
      const D3DXMATRIX& globalMtx = lt.getGlobalMtx();
      d3dLight.Position = D3DXVECTOR3(globalMtx._41, globalMtx._42, globalMtx._43);
      d3dLight.Direction = D3DXVECTOR3(globalMtx._31, globalMtx._32, globalMtx._33); // the global matrix's look vector

      m_d3Device.SetLight(i, &d3dLight);
      m_d3Device.LightEnable(i, true);
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DRendererImpl::updateLightInfo(unsigned int lightIdx, Light& lt)
{
   D3DLIGHT9& light = m_cachedLights[lightIdx].light;
   switch(lt.getType())
   {
   case Light::LT_POINT:       light.Type = D3DLIGHT_POINT;        break;
   case Light::LT_SPOT:        light.Type = D3DLIGHT_SPOT;         break;
   case Light::LT_DIRECTIONAL: light.Type = D3DLIGHT_DIRECTIONAL;  break;
   default:                    light.Type = D3DLIGHT_POINT;        break;
   }

   light.Ambient = (const D3DCOLORVALUE&)(lt.getAmbientColor());
   light.Specular = (const D3DCOLORVALUE&)(lt.getSpecularColor());
   light.Diffuse = (const D3DCOLORVALUE&)(lt.getDiffuseColor());
   light.Range = lt.getRange();
   light.Falloff = lt.getFalloff();
   light.Theta = lt.getTheta();
   light.Phi = lt.getPhi();
   light.Attenuation0 = lt.getConstantAtt();
   light.Attenuation1 = lt.getLinearAtt();
   light.Attenuation2 = lt.getQuadraticAtt();

   // store the present light change index
   m_cachedLights[lightIdx].lastChangeIdx = lt.getChangeIdx();
}

///////////////////////////////////////////////////////////////////////////////

void D3DRendererImpl::disableLights(const Array<Light*>& lights)
{
   unsigned int lightsCount = min(lights.size(), m_maxLightsCount);
   for (unsigned int i = 0; i < lightsCount; ++i)
   {
      m_d3Device.LightEnable(i, false);
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DRendererImpl::setMatrices(const D3DXMATRIX& viewMtx, 
                                  const D3DXMATRIX& projMtx)
{
   m_d3Device.SetTransform(D3DTS_VIEW, &viewMtx);
   m_d3Device.SetTransform(D3DTS_PROJECTION, &projMtx);
}

///////////////////////////////////////////////////////////////////////////////

void D3DRendererImpl::setObjectMatrices(const Array<D3DXMATRIX>& matrices)
{
   unsigned int matricesCount = matrices.size();
   for (unsigned char i = 0; i < matricesCount; ++i)
   {
      m_d3Device.SetTransform(D3DTS_WORLDMATRIX(i), &(matrices[i]));
   }
   m_d3Device.SetRenderState(D3DRS_VERTEXBLEND, matricesCount - 1);
}

///////////////////////////////////////////////////////////////////////////////

void D3DRendererImpl::setTransparency(bool enable)
{
   if (enable)
   {
      m_d3Device.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
      m_d3Device.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
      m_d3Device.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

      // TODO: jak tylko beda drzewa BSP do prezchowywania obiektow przezroczystych,
      //       wywalic wylaczanie z bufora tutaj...
      m_d3Device.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
   }
   else
   {
      m_d3Device.SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
      m_d3Device.SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
      m_d3Device.SetRenderState(D3DRS_ZWRITEENABLE, TRUE); 
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DRendererImpl::setLRP(LightReflectingProperties& lrp)
{
   m_material.Ambient = reinterpret_cast<const D3DCOLORVALUE&>(lrp.getAmbientColor());
   m_material.Diffuse = reinterpret_cast<const D3DCOLORVALUE&>(lrp.getDiffuseColor());
   m_material.Specular = reinterpret_cast<const D3DCOLORVALUE&>(lrp.getSpecularColor());
   m_material.Emissive = reinterpret_cast<const D3DCOLORVALUE&>(lrp.getEmissiveColor());
   m_material.Power = lrp.getPower();
   m_d3Device.SetMaterial(&m_material);
}

///////////////////////////////////////////////////////////////////////////////

void D3DRendererImpl::setTexture(unsigned char stageIndex, 
                                 Texture& texture)
{
   TTextureImpl<IDirect3DTexture9>* typedImpl = dynamic_cast<TTextureImpl<IDirect3DTexture9>*> (&texture.getImpl());
   if (typedImpl == NULL)
   {
      m_d3Device.SetTexture(stageIndex, NULL);
   }
   else
   {
      m_d3Device.SetTexture(stageIndex, &(typedImpl->get()));
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DRendererImpl::setColorOperation(unsigned char stageIndex, 
                                        MaterialOperation& colorOp)
{
   m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLORARG1, translateArg(colorOp.getArg1()));
   m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLORARG2, translateArg(colorOp.getArg2()));

   switch(colorOp.getOperationCode())
   {
   case MOP_SELECT_ARG1:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
         break;
      }

   case MOP_SELECT_ARG2:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
         break;
      }

   case MOP_MULTIPLY:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLOROP, D3DTOP_MODULATE);
         break;
      }

   case MOP_MULTIPLY_2X:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
         break;
      }

   case MOP_MULTIPLY_4X:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLOROP, D3DTOP_MODULATE4X);
         break;
      }

   case MOP_ADD:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLOROP, D3DTOP_ADD);
         break;
      }

   case MOP_ADD_SIGNED:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);
         break;
      }

   case MOP_ADD_SIGNED_2X:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLOROP, D3DTOP_ADDSIGNED2X);
         break;
      }

   case MOP_ADD_SMOOTH:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLOROP, D3DTOP_ADDSMOOTH);
         break;
      }

   case MOP_SUBTRACT:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
         break;
      }

   case MOP_MULTIPLY_ADD:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD);
         break;
      }

   case MOP_DISABLE: // fallthrough
   default:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_COLOROP, D3DTOP_DISABLE);
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DRendererImpl::setAlphaOperation(unsigned char stageIndex, 
                                        MaterialOperation& alphaOp)
{
   m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAARG1, translateArg(alphaOp.getArg1()));
   m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAARG2, translateArg(alphaOp.getArg2()));

   switch(alphaOp.getOperationCode())
   {
   case MOP_SELECT_ARG1:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
         break;
      }

   case MOP_SELECT_ARG2:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
         break;
      }

   case MOP_MULTIPLY:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
         break;
      }

   case MOP_MULTIPLY_2X:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
         break;
      }

   case MOP_MULTIPLY_4X:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAOP, D3DTOP_MODULATE4X);
         break;
      }

   case MOP_ADD:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAOP, D3DTOP_ADD);
         break;
      }

   case MOP_ADD_SIGNED:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAOP, D3DTOP_ADDSIGNED);
         break;
      }

   case MOP_ADD_SIGNED_2X:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAOP, D3DTOP_ADDSIGNED2X);
         break;
      }

   case MOP_ADD_SMOOTH:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAOP, D3DTOP_ADDSMOOTH);
         break;
      }

   case MOP_SUBTRACT:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAOP, D3DTOP_SUBTRACT);
         break;
      }

   case MOP_MULTIPLY_ADD:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAOP, D3DTOP_MULTIPLYADD);
         break;
      }

   case MOP_DISABLE: // falthrough
   default:
      {
         m_d3Device.SetTextureStageState(stageIndex, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

DWORD D3DRendererImpl::translateArg(SourceCode code) const
{
   switch (code)
   {
   case SC_NONE:     return D3DTA_SELECTMASK;
   case SC_LRP:      return D3DTA_DIFFUSE;
   case SC_TEXTURE:  return D3DTA_TEXTURE;
   case SC_PREV:     return D3DTA_CURRENT;
   default: throw std::runtime_error("Invalid argument index");
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DRendererImpl::setCoordsOperation(unsigned char stageIndex, 
                                         CoordinatesOperation& coordsOp)
{
   DWORD d3dCode;
   switch(coordsOp.getCode())
   {
      case CC_WRAP:        d3dCode = D3DTADDRESS_WRAP;         break;
      case CC_MIRROR:      d3dCode = D3DTADDRESS_MIRROR;       break;
      case CC_CLAMP:       d3dCode = D3DTADDRESS_CLAMP;        break;
      case CC_BORDER:      d3dCode = D3DTADDRESS_BORDER;       break;
      case CC_MIRRORONCE:  d3dCode = D3DTADDRESS_MIRRORONCE;   break;
      default:             d3dCode = D3DTADDRESS_WRAP;         break;
   }

   m_d3Device.SetSamplerState(stageIndex, D3DSAMP_ADDRESSU, d3dCode);
   m_d3Device.SetSamplerState(stageIndex, D3DSAMP_ADDRESSV, d3dCode);
}

///////////////////////////////////////////////////////////////////////////////
