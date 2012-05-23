/// @file   core-AI/SteeringBehavior.h
/// @brief  steering behavior interface.
#pragma once


///////////////////////////////////////////////////////////////////////////////

struct Vector;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class defines a single steering behavior - an algorithm that
 * calculates a steering force that can be used to propel a physical body.
 */
class SteeringBehavior
{
public:
   virtual ~SteeringBehavior() {}

   /**
    * The method calculates a velocity that should be applied
    * so that it acts as if it was behaving according to this behavior's
    * logic.
    *
    * @param timeElapsed   time (in seconds) that has elapsed since 
    *                      the last time this method was called
    * @return              velocity that should be applied 
    *                      to the controlled body
    */
   virtual void calculateVelocity( float timeElapsed, Vector& outVelocity ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
