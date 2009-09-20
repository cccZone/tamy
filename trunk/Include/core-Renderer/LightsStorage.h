#pragma once

/// @file   core-Renderer\LightsStorage.h
/// @brief  an interface for querying lights by the RenderLights action

#include <d3dx9.h>
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class Light;
class BoundingVolume;

///////////////////////////////////////////////////////////////////////////////

/**
 * RenderLights action uses this interface to gather lights
 * influencing a point.
 */
class LightsStorage
{
public:
   virtual ~LightsStorage() {}

   /**
    * Query the lights affecting the specified point.
    *
    * @param volume     volume in space lights for which we want to check
    * @param outLights  upon method return this array will contain lights
    *                   that shine upon the specified point
    */
   virtual void query(const BoundingVolume& volume, Array<Light*>& outLights) = 0;
};

///////////////////////////////////////////////////////////////////////////////
