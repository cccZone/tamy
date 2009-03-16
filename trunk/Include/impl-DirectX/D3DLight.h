#pragma once

#include "core-Renderer\Light.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

struct D3DXVECTOR3;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a DirectX representation of a light
 */
class D3DLight : public Light
{
private:
   static UINT s_nextAvailableNo;
   UINT m_lightNo;

   IDirect3DDevice9& m_d3Device;
   D3DLIGHT9 m_lightDesc;
   bool m_enabled;

public:
   D3DLight(const std::string& name, IDirect3DDevice9& d3Device);

   void enable(bool val);
   bool isEnabled() const {return m_enabled;}

   void setType(LightType type);
   void setAmbientColor(const Color& ambient);
   void setDiffuseColor(const Color& diffuse);
   void setSpecularColor(const Color& specular);
   void setRange(float range);
   void setAttenuation(float constant, float linear, float quadratic);
   void setSpotlightFeatures(float falloff, float theta, float phi);

private:
   void refreshLight();
};

///////////////////////////////////////////////////////////////////////////////
