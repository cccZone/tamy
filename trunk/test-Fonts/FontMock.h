#pragma once

#include "ext-Fonts\Font.h"
#include "GraphicalEntityMock.h"
#include "core-Renderer\Material.h"
#include "LightReflectingPropertiesStub.h"
#include "MaterialOperationImplementationMock.h"
#include "TransparencyEnablerStub.h"
#include "RenderingTargetsPolicyMock.h"


///////////////////////////////////////////////////////////////////////////////

class FontMock : public Font
{
private:
   std::vector<unsigned char> m_chars;
   GraphicalEntityMock m_mockEntity;

   RenderingTargetsPolicyMock s_policy;
   MaterialOperationImplementationMock m_matOp;
   TransparencyEnablerStub m_transparencyEnabler;
   Material* m_material;

public:
   FontMock() 
      : Font("FontMock") 
   {
      m_material = new Material("someMaterial", 
                                s_policy,
                                new LightReflectingPropertiesStub(),
                                m_matOp, m_matOp,
                                m_transparencyEnabler);
   }

   ~FontMock()
   {
      delete m_material;
      m_material = NULL;
   }

   void reset()
   {
      m_chars.clear();
   }

   AbstractGraphicalEntity& getChar(unsigned char c) 
   {
      m_chars.push_back(c);
      return m_mockEntity;
   }

   char getRequestedChar(unsigned int idx)
   {
      return m_chars.at(idx);
   }

   float getCharWidth(unsigned char c) const
   {
      return 10;
   }

   const Material& getMaterial() const
   {
      return *m_material;
   }
};

///////////////////////////////////////////////////////////////////////////////
