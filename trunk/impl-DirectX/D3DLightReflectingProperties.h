#pragma once

#include "LightReflectingProperties.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This is the DirectX implementation of a Material
 */
class D3DLightReflectingProperties : public LightReflectingProperties
{
private:
   IDirect3DDevice9& m_d3Device;
   D3DMATERIAL9 m_material;
   bool m_transparent;

public:
   D3DLightReflectingProperties(IDirect3DDevice9& d3Device);
   ~D3DLightReflectingProperties();

   bool operator==(const LightReflectingProperties& rhs) const;

   void setAmbientColor(const Color& ambient);
   void setDiffuseColor(const Color& diffuse);
   void setSpecularColor(const Color& specular);
   void setEmissiveColor(const Color& emissive);
   void setPower(float val);

   bool isTransparent() const {return m_transparent;}

   void setForRendering();

private:
   bool checkTransparency(const Color& color) const;
};

///////////////////////////////////////////////////////////////////////////////
