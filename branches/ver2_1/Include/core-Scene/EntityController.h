#pragma once

/// @file   core-Scene\EntityController.h
/// @brief  object that will control an entity


///////////////////////////////////////////////////////////////////////////////

/**
 * This interface established means for controlling entities as time passes by.
 */
class EntityController
{
public:
   virtual ~EntityController() {}

   /**
    * This method should contain entity control logics (dependent or 
    * independent of time flow - whatever suits your needs).
    *
    * @param timeElapsed   amount of time elapsed since the last time this
    *                      method was called
    */
   virtual void update(float timeElapsed) = 0;
};

///////////////////////////////////////////////////////////////////////////////
