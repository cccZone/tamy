#pragma once

#include "core-Renderer\RenderingTarget.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace RegularTests
{
   class RenderingTargetMock : public RenderingTarget
   {
   private:
      int m_renderIdx;

   public:
      RenderingTargetMock() 
         : m_renderIdx(-1)
      {}

      void use(unsigned char idx) { m_renderIdx = idx;}

      int getRenderingIdx() 
      {
         int result = m_renderIdx;
         m_renderIdx = -1;
         return result;
      }
   };
}

///////////////////////////////////////////////////////////////////////////////

namespace RendereringPipelineTests
{
   class RenderingTargetMock : public RenderingTarget
   {
   private:
      std::string m_id;
      std::vector<std::string>& m_seqLog;

   public:
      RenderingTargetMock(const std::string& id,
                          std::vector<std::string>& seqLog) 
         : m_id(id), m_seqLog(seqLog)
      {}

      void use(unsigned char idx) 
      {
         char tmpStr[1024];
         sprintf_s(tmpStr, 1024, "%s - %d", m_id.c_str(), idx);
         m_seqLog.push_back(tmpStr);
      }
   };
}

///////////////////////////////////////////////////////////////////////////////
