#pragma once

#include "core-Renderer\RenderingMechanism.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingMechanismMock : public RenderingMechanism
{
private:
   std::string m_id;
   std::vector<std::string>& m_seqLog;

public:
   RenderingMechanismMock(const std::string& id,
                          std::vector<std::string>& seqLog,
                          RenderingTargetsPolicy* policy) 
         : RenderingMechanism(policy), m_id(id), m_seqLog(seqLog)
   {}

   void render() 
   {
      char tmpStr[64];
      sprintf_s(tmpStr, 64, "%s - render", m_id.c_str());
      m_seqLog.push_back(tmpStr);
   }

};

///////////////////////////////////////////////////////////////////////////////
