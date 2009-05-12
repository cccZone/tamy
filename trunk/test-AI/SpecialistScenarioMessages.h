#pragma once

#include "core-AI\Message.h"
#include "DemolitionMan.h"


///////////////////////////////////////////////////////////////////////////////

class ArmMe : public Message,
              public TMessage<DemolitionMan>
{
private:
   ExplosiveCharge& m_charge;

public:
   ArmMe(ExplosiveCharge& charge) : m_charge(charge) {}

   Message* clone() {return new ArmMe(*this);}

   void perform(DemolitionMan& specialist, Messagable& sender)
   {
      specialist.arm(m_charge);
   }
};

///////////////////////////////////////////////////////////////////////////////
