#include "Rambo.h"
#include "RamboScenarioMessages.h"
#include "core-AI\MessageDispatcher.h"
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

void Rambo::engageEnemy()
{
   assert(m_enemySpotted != NULL);

   dispatcher().send(*this, *m_enemySpotted, new ReduceHealth(), 0);
}

///////////////////////////////////////////////////////////////////////////////
