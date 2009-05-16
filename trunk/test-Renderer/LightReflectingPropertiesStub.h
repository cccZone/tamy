#pragma once

#include "core-Renderer\LightReflectingProperties.h"
#include <list>
#include <string>
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

class LightReflectingPropertiesStub : public LightReflectingProperties
{
private:
   std::list<std::string>* m_messageSink;
   int m_id;

public:
   LightReflectingPropertiesStub() : m_messageSink(NULL), m_id(0) {}

   LightReflectingPropertiesStub(std::list<std::string>& messageSink, int id = 0) 
      : m_messageSink(&messageSink), m_id(id) {}

   bool operator==(const LightReflectingProperties& rhs) const 
   {
      const LightReflectingPropertiesStub* lrp = dynamic_cast<const LightReflectingPropertiesStub*> (&rhs);
      if (lrp == NULL) {return false;}
      return m_id == lrp->m_id;
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
         std::stringstream msg;
         msg << "Set light reflecting properties " << m_id;
         m_messageSink->push_back(msg.str());
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
