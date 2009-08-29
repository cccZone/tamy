#pragma once

#include "core-Renderer\RenderingTargetsPolicy.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace RegularTests
{
   class RenderingTargetsPolicyMock : public RenderingTargetsPolicy
   {
   public:
      unsigned int setTargets(unsigned int passIdx)
      {
         return 0;
      }

      unsigned int getDefinedPassesCount() const {return 0;}
   };
}

///////////////////////////////////////////////////////////////////////////////

namespace GraphicalEffectTests
{
   class RenderingTargetsPolicyMock : public RenderingTargetsPolicy
   {
   private:
      std::vector<std::string>& m_seqLog;

   public:
      RenderingTargetsPolicyMock(std::vector<std::string>& seqLog)
         : m_seqLog(seqLog)
      {}

      unsigned int setTargets(unsigned int passIdx)
      {
         char tmpStr[64];
         sprintf_s(tmpStr, 64, "setting targets for pass %d", passIdx);
         m_seqLog.push_back(tmpStr);

         return 0;
      }

      unsigned int getDefinedPassesCount() const {return 0;}
   };
}

///////////////////////////////////////////////////////////////////////////////

namespace CallSeqTrack
{
   class RenderingTargetsPolicyMock : public RenderingTargetsPolicy
   {
   private:
      std::vector<std::string>& m_seqLog;

   public:
      RenderingTargetsPolicyMock(std::vector<std::string>& seqLog)
         : m_seqLog(seqLog)
      {}

      unsigned int setTargets(unsigned int passIdx)
      {
         m_seqLog.push_back("Setting rendering targets");
         return 1;
      }

      unsigned int getDefinedPassesCount() const {return 1;}
   };
}

///////////////////////////////////////////////////////////////////////////////
