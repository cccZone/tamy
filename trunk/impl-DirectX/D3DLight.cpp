#include "impl-DirectX\D3DLight.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

UINT D3DLight::s_nextAvailableNo = 0;

///////////////////////////////////////////////////////////////////////////////

D3DLight::D3DLight(const std::string& name, IDirect3DDevice9& d3Device)
      : Light(name),
      m_d3Device(d3Device),
      m_lightNo(s_nextAvailableNo++),
      m_enabled(false)
{
   ZeroMemory(&m_lightDesc, sizeof(D3DLIGHT9));
}

///////////////////////////////////////////////////////////////////////////////

void D3DLight::enable(bool val)
{
   m_enabled = val;
   m_d3Device.LightEnable(m_lightNo, m_enabled);

   // if we're turning the light off, there's no need to reset it (which costs like A LOT),
   // we'll simply put it off till the next time the light is turned on
   if (val == false) {return;}

   const D3DXMATRIX& globalMtx = getGlobalMtx();
   m_lightDesc.Position = D3DXVECTOR3(globalMtx._41, globalMtx._42, globalMtx._43);
   m_lightDesc.Direction = D3DXVECTOR3(globalMtx._31, globalMtx._32, globalMtx._33); // the global matrix's look vector

   m_d3Device.SetLight(m_lightNo, &m_lightDesc);
}

///////////////////////////////////////////////////////////////////////////////

void D3DLight::refreshLight()
{
   m_d3Device.SetLight(m_lightNo, &m_lightDesc);
   m_d3Device.LightEnable(m_lightNo, true);
}

///////////////////////////////////////////////////////////////////////////////

void D3DLight::setType(LightType type)
{
   switch(type)
   {
   case Light::LT_POINT:       m_lightDesc.Type = D3DLIGHT_POINT;        break;
   case Light::LT_SPOT:        m_lightDesc.Type = D3DLIGHT_SPOT;         break;
   case Light::LT_DIRECTIONAL: m_lightDesc.Type = D3DLIGHT_DIRECTIONAL;  break;
   default: break;
   }

   if (m_enabled) {refreshLight();}
}

///////////////////////////////////////////////////////////////////////////////

void D3DLight::setAmbientColor(const Color& ambient)
{
   m_lightDesc.Ambient.r = ambient.r;
   m_lightDesc.Ambient.g = ambient.g;
   m_lightDesc.Ambient.b = ambient.b;
   m_lightDesc.Ambient.a = ambient.a;

   if (m_enabled) {refreshLight();}
}

///////////////////////////////////////////////////////////////////////////////

void D3DLight::setDiffuseColor(const Color& diffuse)
{
   m_lightDesc.Diffuse.r = diffuse.r;
   m_lightDesc.Diffuse.g = diffuse.g;
   m_lightDesc.Diffuse.b = diffuse.b;
   m_lightDesc.Diffuse.a = diffuse.a;

   if (m_enabled) {refreshLight();}
}

///////////////////////////////////////////////////////////////////////////////

void D3DLight::setSpecularColor(const Color& specular)
{
   m_lightDesc.Specular.r = specular.r;
   m_lightDesc.Specular.g = specular.g;
   m_lightDesc.Specular.b = specular.b;
   m_lightDesc.Specular.a = specular.a;

   if (m_enabled) {refreshLight();}
}

///////////////////////////////////////////////////////////////////////////////

void D3DLight::setRange(float range)
{
   m_lightDesc.Range = range;

   if (m_enabled) {refreshLight();}
}

///////////////////////////////////////////////////////////////////////////////

void D3DLight::setAttenuation(float constant, float linear, float quadratic)
{
   m_lightDesc.Attenuation0 = constant;
   m_lightDesc.Attenuation1 = linear;
   m_lightDesc.Attenuation2 = quadratic;

   if (m_enabled) {refreshLight();}
}

///////////////////////////////////////////////////////////////////////////////

void D3DLight::setSpotlightFeatures(float falloff, float theta, float phi)
{
   m_lightDesc.Falloff = falloff;
   m_lightDesc.Theta = theta;
   m_lightDesc.Phi = phi;

   if (m_enabled) {refreshLight();}
}

///////////////////////////////////////////////////////////////////////////////

