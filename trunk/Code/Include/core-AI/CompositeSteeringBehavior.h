#pragma once

#include "core-AI\SteeringBehavior.h"
#include "core\FastFloat.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

struct Vector;

///////////////////////////////////////////////////////////////////////////////

/**
 * A composite that allows to combine multiple steering behaviors into one.
 */
class CompositeSteeringBehavior : public SteeringBehavior
{
   DECLARE_ALLOCATOR( CompositeSteeringBehavior, AM_DEFAULT );

private:
   struct BehaviorDesc
   {
      DECLARE_ALLOCATOR( BehaviorDesc, AM_ALIGNED_16 );

      SteeringBehavior* behavior;
      FastFloat weight;

      BehaviorDesc(SteeringBehavior* _behavior, float _weight)
         : behavior(_behavior)
      {
         weight.setFromFloat( _weight );
      }
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
   void add( SteeringBehavior* behavior, float weight );

   // -------------------------------------------------------------------------
   // SteeringBehavior implementation
   // -------------------------------------------------------------------------
   void calculateVelocity( float timeElapsed, Vector& outVelocity );
};

///////////////////////////////////////////////////////////////////////////////
