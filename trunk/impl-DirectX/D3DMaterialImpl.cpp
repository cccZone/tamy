#include "impl-DirectX\D3DMaterialImpl.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

D3DMaterialImpl::D3DMaterialImpl(IDirect3DDevice9& d3Device)
      : m_d3Device(d3Device)
{
}

///////////////////////////////////////////////////////////////////////////////

void D3DMaterialImpl::setTransparency(bool enable)
{
   if (enable)
   {
      m_d3Device.SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
      m_d3Device.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
      m_d3Device.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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

void D3DMaterialImpl::setLRP(LightReflectingProperties& lrp)
{
   m_material.Ambient = reinterpret_cast<const D3DCOLORVALUE&>(lrp.getAmbientColor());
   m_material.Diffuse = reinterpret_cast<const D3DCOLORVALUE&>(lrp.getDiffuseColor());
   m_material.Specular = reinterpret_cast<const D3DCOLORVALUE&>(lrp.getSpecularColor());
   m_material.Emissive = reinterpret_cast<const D3DCOLORVALUE&>(lrp.getEmissiveColor());
   m_material.Power = lrp.getPower();
   m_d3Device.SetMaterial(&m_material);
}

///////////////////////////////////////////////////////////////////////////////

void D3DMaterialImpl::setTexture(unsigned char stageIndex, 
                                 TextureImpl& textureImpl)
{
   TTextureImpl<IDirect3DTexture9>* typedImpl = dynamic_cast<TTextureImpl<IDirect3DTexture9>*> (&textureImpl);
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

void D3DMaterialImpl::setColorOperation(unsigned char stageIndex, 
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

void D3DMaterialImpl::setAlphaOperation(unsigned char stageIndex, 
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

DWORD D3DMaterialImpl::translateArg(SourceCode code) const
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

void D3DMaterialImpl::setCoordsOperation(unsigned char stageIndex, 
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
