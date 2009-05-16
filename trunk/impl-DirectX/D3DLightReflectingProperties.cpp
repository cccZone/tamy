#include "impl-DirectX\D3DLightReflectingProperties.h"


///////////////////////////////////////////////////////////////////////////////

D3DLightReflectingProperties::D3DLightReflectingProperties(IDirect3DDevice9& d3Device)
      : m_d3Device(d3Device)
{
   m_d3Device.AddRef();
   ZeroMemory(&m_material, sizeof(D3DMATERIAL9));
}

///////////////////////////////////////////////////////////////////////////////

D3DLightReflectingProperties::~D3DLightReflectingProperties()
{
   m_d3Device.Release();
}

///////////////////////////////////////////////////////////////////////////////

bool D3DLightReflectingProperties::operator==(const LightReflectingProperties& rhs) const
{
   const D3DLightReflectingProperties* rm = dynamic_cast<const D3DLightReflectingProperties*> (&rhs);
   if (rm == NULL) {return false;}

   return (m_material.Ambient.r == rm->m_material.Ambient.r) &&
          (m_material.Ambient.g == rm->m_material.Ambient.g) &&
          (m_material.Ambient.b == rm->m_material.Ambient.b) &&
          (m_material.Ambient.a == rm->m_material.Ambient.a) &&

          (m_material.Diffuse.r == rm->m_material.Diffuse.r) &&
          (m_material.Diffuse.g == rm->m_material.Diffuse.g) &&
          (m_material.Diffuse.b == rm->m_material.Diffuse.b) &&
          (m_material.Diffuse.a == rm->m_material.Diffuse.a) &&

          (m_material.Specular.r == rm->m_material.Specular.r) &&
          (m_material.Specular.g == rm->m_material.Specular.g) &&
          (m_material.Specular.b == rm->m_material.Specular.b) &&
          (m_material.Specular.a == rm->m_material.Specular.a) &&

          (m_material.Emissive.r == rm->m_material.Emissive.r) &&
          (m_material.Emissive.g == rm->m_material.Emissive.g) &&
          (m_material.Emissive.b == rm->m_material.Emissive.b) &&
          (m_material.Emissive.a == rm->m_material.Emissive.a) &&

          (m_material.Power == rm->m_material.Power);
}

///////////////////////////////////////////////////////////////////////////////

void D3DLightReflectingProperties::setAmbientColor(const Color& ambient)
{
   m_material.Ambient.r = ambient.r;
   m_material.Ambient.g = ambient.g;
   m_material.Ambient.b = ambient.b;
   m_material.Ambient.a = ambient.a;
}

///////////////////////////////////////////////////////////////////////////////

void D3DLightReflectingProperties::setDiffuseColor(const Color& diffuse)
{
   m_material.Diffuse.r = diffuse.r;
   m_material.Diffuse.g = diffuse.g;
   m_material.Diffuse.b = diffuse.b;
   m_material.Diffuse.a = diffuse.a;
}

///////////////////////////////////////////////////////////////////////////////

void D3DLightReflectingProperties::setSpecularColor(const Color& specular)
{
   m_material.Specular.r = specular.r;
   m_material.Specular.g = specular.g;
   m_material.Specular.b = specular.b;
   m_material.Specular.a = specular.a;
}

///////////////////////////////////////////////////////////////////////////////

void D3DLightReflectingProperties::setEmissiveColor(const Color& emissive)
{
   m_material.Emissive.r = emissive.r;
   m_material.Emissive.g = emissive.g;
   m_material.Emissive.b = emissive.b;
   m_material.Emissive.a = emissive.a;
}

///////////////////////////////////////////////////////////////////////////////

void D3DLightReflectingProperties::setPower(float val)
{
   m_material.Power = val;
}

///////////////////////////////////////////////////////////////////////////////

void D3DLightReflectingProperties::setForRendering()
{
   m_d3Device.SetMaterial(&m_material);
}

///////////////////////////////////////////////////////////////////////////////
