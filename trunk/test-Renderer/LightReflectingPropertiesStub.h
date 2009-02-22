#pragma once

#include "LightReflectingProperties.h"
#include <list>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class LightReflectingPropertiesStub : public LightReflectingProperties
{
private:
   std::list<std::string>* m_messageSink;

public:
   LightReflectingPropertiesStub() : m_messageSink(NULL) {}

   LightReflectingPropertiesStub(std::list<std::string>& messageSink) : m_messageSink(&messageSink) {}

   bool operator==(const LightReflectingProperties& rhs) const 
   {
      const LightReflectingPropertiesStub* lrp = dynamic_cast<const LightReflectingPropertiesStub*> (&rhs);
      return (lrp != NULL);
   }

   void setAmbientColor(const Color& ambient) {}
   void setDiffuseColor(const Color& diffuse) {}
   void setSpecularColor(const Color& specular) {}
   void setEmissiveColor(const Color& emissive){}
   void setPower(float val) {}

   void setForRendering() 
   {
      if (m_messageSink != NULL)
      {
         m_messageSink->push_back("Set light reflecting properties");
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
