#pragma once

#include "core-Renderer\Light.h"


///////////////////////////////////////////////////////////////////////////////

class LightMock : public Light
{
private:
   bool m_enabled;
   bool m_hasBeenEnabled;

public:
   LightMock() : Light("light"), m_enabled(false), m_hasBeenEnabled(false) {}

   void enable(bool val) 
   {
      m_enabled = val;
      m_hasBeenEnabled |= val;
   }

   bool isEnabled() const {return m_enabled;}
   bool hasBeenEnabled() const {return m_hasBeenEnabled;}

   void setType(LightType type) {}
   void setAmbientColor(const Color& ambient) {}
   void setDiffuseColor(const Color& diffuse) {}
   void setSpecularColor(const Color& specular) {}
   void setRange(float range) {}
   void setAttenuation(float constant, float linear, float quadratic) {}
   void setSpotlightFeatures(float falloff, float theta, float phi) {}
};

///////////////////////////////////////////////////////////////////////////////
