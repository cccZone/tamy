#pragma once

#include "core-AI\SteeringBehavior.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * A composite that allows to combine multiple steering behaviors into one.
 */
class CompositeSteeringBehavior : public SteeringBehavior
{
private:
   struct BehaviorDesc
   {
      SteeringBehavior* behavior;
      float weight;

      BehaviorDesc(SteeringBehavior* _behavior,
                   float _weight)
         : behavior(_behavior)
         , weight(_weight)
      {}
   };

private:
   std::vector<BehaviorDesc*> m_behaviors;

public:
   ~CompositeSteeringBehavior();

   /**
    * Adds a new behavior to the composite.
    *
    * @param behavior   behavior to be added
    * @param weight     how important is the force it calculates to the
    *                   total force returned by the composite
    */
   void add(SteeringBehavior* behavior, float weight);

   D3DXVECTOR3 calculateVelocity(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////
