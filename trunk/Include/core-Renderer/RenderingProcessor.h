#pragma once

#include <list>
#include "core-Renderer\RenderingCommand.h"
#include "core-Renderer\BatchingStrategy.h"


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalNode;
class Material;
typedef AbstractGraphicalNode* AbstractGraphicalNodeP;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class takes care of translating whatever's inside the graphical
 * nodes into a list of rendering commands the renderer can execute
 * in order to create an image on the screen.
 */
class RenderingProcessor
{
public:
   DWORD translate(AbstractGraphicalNodeP* nodesToRender, const DWORD& nodesArraySize,
                   RenderingCommand* renderingCommands, const DWORD& commandsArraySize);

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
