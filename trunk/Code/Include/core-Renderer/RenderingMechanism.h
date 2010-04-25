#pragma once


///////////////////////////////////////////////////////////////////////////////

class RenderingTargetsPolicy;

///////////////////////////////////////////////////////////////////////////////

/** 
 * Rendering pipeline is combined from the rendering mechanisms.
 * Each mechanism is a building brick that adds some aspect
 * to the final rendering of a scene.
 */ 
class RenderingMechanism
{
public:
   virtual ~RenderingMechanism() {}

   /**
    * Performs the rendering operation defined by the mechanism.
    */
   virtual void render() = 0;
};

///////////////////////////////////////////////////////////////////////////////
