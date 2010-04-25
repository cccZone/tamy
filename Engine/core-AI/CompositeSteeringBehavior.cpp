#include "core-AI\CompositeSteeringBehavior.h"
#include <d3dx9.h>
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

CompositeSteeringBehavior::~CompositeSteeringBehavior()
{
   unsigned int count = m_behaviors.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_behaviors[i]->behavior;
      delete m_behaviors[i];
   }
   m_behaviors.clear();
}

///////////////////////////////////////////////////////////////////////////////

void CompositeSteeringBehavior::add(SteeringBehavior* behavior, 
                                    float weight)
{
   if (behavior == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a SteeringBehavior instance");
   }

   m_behaviors.push_back(new BehaviorDesc(behavior, weight));
}

///////////////////////////////////////////////////////////////////////////////

D3DXVECTOR3 CompositeSteeringBehavior::calculateVelocity(float timeElapsed)
{
   D3DXVECTOR3 velocity(0, 0, 0);

   unsigned int count = m_behaviors.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      velocity += m_behaviors[i]->behavior->calculateVelocity(timeElapsed) * m_behaviors[i]->weight;
   }

   return velocity;
}

///////////////////////////////////////////////////////////////////////////////
