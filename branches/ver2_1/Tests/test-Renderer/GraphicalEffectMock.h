#pragma once

#include "core-Renderer\GraphicalEffect.h"
#include "core-Renderer\ManagedTexture.h"
#include <d3dx9.h>
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class GraphicalEffectMock : public GraphicalEffect
{
private:
   std::vector<std::string>& m_callSequenceVec;
   unsigned int m_passesCount;

public:
   GraphicalEffectMock(std::vector<std::string>& callSequenceVec) 
         : GraphicalEffect("GraphicalEffectMock"),
         m_callSequenceVec(callSequenceVec),
         m_passesCount(1)
   {}

   void setTechnique(const std::string& material) {}
   void setBool(const std::string& paramName, bool val)
   {
      char tmpStr[128];
      sprintf_s(tmpStr, 128, "setting param %s", paramName.c_str());
      m_callSequenceVec.push_back(tmpStr);
   }
   void setInt(const std::string& paramName, int val)
   {
      char tmpStr[128];
      sprintf_s(tmpStr, 128, "setting param %s = %d", paramName.c_str(), val);
      m_callSequenceVec.push_back(tmpStr);
   }
   void setInt(const std::string& paramName, const int* arr, unsigned int size){ }
   void setFloat(const std::string& paramName, float val) 
   {
      char tmpStr[128];
      sprintf_s(tmpStr, 128, "setting param %s = %.2f", paramName.c_str(), val);
      m_callSequenceVec.push_back(tmpStr);
   }
   void setFloat(const std::string& paramName, const float* arr, unsigned int size){ }
   void setMtx(const std::string& paramName, const D3DXMATRIX& val)
   {
      char tmpStr[128];
      sprintf_s(tmpStr, 128, "setting param %s", paramName.c_str());
      m_callSequenceVec.push_back(tmpStr);
   }
   void setMtx(const std::string& paramName, const D3DXMATRIX* arr, unsigned int size)
   {
      char tmpStr[128];
      sprintf_s(tmpStr, 128, "setting param %s", paramName.c_str());
      m_callSequenceVec.push_back(tmpStr);
   }
   void setString(const std::string& paramName, const std::string& val){ }
   void setTexture(const std::string& paramName, Texture& val){ }
   void setVec4(const std::string& paramName, const D3DXVECTOR4& val){ }
   void setVec4(const std::string& paramName, const D3DXVECTOR4* arr, unsigned int size){ }

   void setPassesCount(unsigned int count) {m_passesCount = count;}

protected:
   unsigned int beginRendering()
   {
      m_callSequenceVec.push_back("GraphicalEffect::beginRendering");
      return m_passesCount;
   }

   void endRendering()
   {
      m_callSequenceVec.push_back("GraphicalEffect::endRendering");
   }

   void beginPass(const unsigned int& passIdx) 
   {
      char tmpStr[128];
      sprintf_s(tmpStr, 128, "GraphicalEffect::beginPass - %d", passIdx);
      m_callSequenceVec.push_back(tmpStr);
   }

   void endPass(const unsigned int& passIdx) 
   {
      char tmpStr[128];
      sprintf_s(tmpStr, 128, "GraphicalEffect::endPass - %d", passIdx);
      m_callSequenceVec.push_back(tmpStr);
   }
};

///////////////////////////////////////////////////////////////////////////////
