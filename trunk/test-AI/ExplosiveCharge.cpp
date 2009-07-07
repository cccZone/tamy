#include "ExplosiveCharge.h"
#include "SpecialistScenarioMessages.h"


///////////////////////////////////////////////////////////////////////////////

void ExplosiveCharge::armMe(Messagable& agent)
{
   dispatcher().send(*this, agent, new ArmMe(*this), 0);
}

///////////////////////////////////////////////////////////////////////////////
