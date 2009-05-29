#pragma once

#include <windows.h>


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
   void translate(AbstractGraphicalNodeP* nodesToRender, const DWORD& nodesArraySize);
};

///////////////////////////////////////////////////////////////////////////////
