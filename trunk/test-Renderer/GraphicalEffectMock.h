#pragma once

#include "core-Renderer\GraphicalEffect.h"
#include "core-Renderer\ManagedTexture.h"
#include "core-Renderer\EffectDataSource.h"
#include "RenderingTargetsPolicyMock.h"
#include <d3dx9.h>
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class GraphicalEffectMock : public GraphicalEffect
{
private:
   static RegularTests::RenderingTargetsPolicyMock s_policy;
   std::vector<std::string>& m_callSequenceVec;

public:
   GraphicalEffectMock(EffectDataSource* dataSource,
                       std::vector<std::string>& callSequenceVec) 
         : GraphicalEffect("GraphicalEffectMock", s_policy, false, dataSource),
         m_callSequenceVec(callSequenceVec)
   {}

   void setTechnique(const std::string& technique) {}
   void set(const std::string& paramName, bool val){ }
   void set(const std::string& paramName, int val){ }
   void set(const std::string& paramName, const Array<int>& val){ }
   void set(const std::string& paramName, float val){ }
   void set(const std::string& paramName, const Array<float>& val){ }
   void set(const std::string& paramName, const D3DXMATRIX& val){ }
   void set(const std::string& paramName, const Array<D3DXMATRIX>& val){ }
   void set(const std::string& paramName, const std::string& val){ }
   void set(const std::string& paramName, Texture& val){ }
   void set(const std::string& paramName, const D3DXVECTOR4& val){ }
   void set(const std::string& paramName, const Array<D3DXVECTOR4>& val){ }

protected:
   unsigned int onBeginRendering()
   {
      m_callSequenceVec.push_back("GraphicalEffect::beginRendering");
      return 1;
   }

   void endRendering()
   {
      m_callSequenceVec.push_back("GraphicalEffect::endRendering");
   }

   void beginPass(const unsigned int& passIdx) {}

   void endPass(const unsigned int& passIdx) {}
};

///////////////////////////////////////////////////////////////////////////////

class EffectDataSourceMock : public EffectDataSource
{
private:
   std::vector<std::string>& m_callSequenceVec;

public:
   EffectDataSourceMock(std::vector<std::string>& callSequenceVec) 
         : m_callSequenceVec(callSequenceVec)
   {}
public:
   void setData(GraphicalEffect& effect)
   {
      m_callSequenceVec.push_back("EffectDataSource::setData");
   }
};

///////////////////////////////////////////////////////////////////////////////