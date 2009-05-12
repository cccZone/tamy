#pragma once

#include "core-AI\Agent.h"


///////////////////////////////////////////////////////////////////////////////

class MessageDispatcher;

///////////////////////////////////////////////////////////////////////////////

class ExplosiveCharge : public Agent<ExplosiveCharge>
{
private:
   bool m_armed;

public:
   ExplosiveCharge() 
         : Agent("explosiveCharge"),
         m_armed(false) 
   {}

   void armMe(Messagable& agent);

   bool isArmed() const {return m_armed;}
   void arm() {m_armed = true;}
};

///////////////////////////////////////////////////////////////////////////////
