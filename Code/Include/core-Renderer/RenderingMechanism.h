#pragma once


///////////////////////////////////////////////////////////////////////////////

class Renderer;

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
    * Initializes the mechanism with the renderer that will be using it.
    *
    * @param renderer
    */
   virtual void initialize( Renderer& renderer ) = 0;

   /**
    * Performs the rendering operation defined by the mechanism.
    */
   virtual void render() = 0;
};

///////////////////////////////////////////////////////////////////////////////
