#include "core-AI\CompositeSteeringBehavior.h"
#include "core/Vector.h"
#include "core/Assert.h"


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
      ASSERT_MSG( false, "NULL pointer instead a SteeringBehavior instance" );
      return;
   }

   m_behaviors.push_back(new BehaviorDesc(behavior, weight));
}

///////////////////////////////////////////////////////////////////////////////

void CompositeSteeringBehavior::calculateVelocity( float timeElapsed, Vector& outVelocity )
{
   outVelocity = Vector::ZERO;

   unsigned int count = m_behaviors.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      Vector vel = Vector::ZERO;
      m_behaviors[i]->behavior->calculateVelocity( timeElapsed, vel );
      vel.mul( m_behaviors[i]->weight );
      outVelocity.add( vel );
   }
}

///////////////////////////////////////////////////////////////////////////////
