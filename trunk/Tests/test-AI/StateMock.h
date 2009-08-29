#pragma once

#include "core-AI\State.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class StateMock : public State<std::string>
{
private:
   std::string m_stateName;

public:
   StateMock(std::string& log, const std::string& stateName)
         : State(log), m_stateName(stateName)
   {
   }

   void onEnter()
   {
      m_agent += m_stateName + "-Entered; ";
   }

   void onUpdate()
   {
      m_agent += m_stateName + "-Updated; ";
   }

   void onExit()
   {
      m_agent += m_stateName + "-Exited; ";
   }
};

///////////////////////////////////////////////////////////////////////////////
