#pragma once

#include <windows.h>
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class takes care of translating whatever's inside the graphical
 * nodes into a list of rendering commands the renderer can execute
 * in order to create an image on the screen.
 */
class RenderingProcessor
{
public:
   void translate(Array<AbstractGraphicalNode*>& nodesToRender);
};

///////////////////////////////////////////////////////////////////////////////
