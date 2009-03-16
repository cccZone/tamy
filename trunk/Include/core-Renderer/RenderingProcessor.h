#pragma once

#include <list>
#include "core-Renderer\RenderingCommand.h"
#include "core-Renderer\BatchingStrategy.h"


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalNode;
class Material;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class takes care of translating whatever's inside the graphical
 * nodes into a list of rendering commands the renderer can execute
 * in order to create an image on the screen.
 */
class RenderingProcessor
{
public:
   std::list<RenderingCommand> translate(const BatchedNodes& nodesToRender);

protected:
   /**
    * This method is supposed to create a command that will
    * set a material on a device
    */
   inline RenderingCommand setMaterial(Material& material);

   /**
    * This method is supposed to create a command that will
    * render an entity on the screen
    */
   inline RenderingCommand renderEntity(AbstractGraphicalNode& graphicalNode);
};

///////////////////////////////////////////////////////////////////////////////
