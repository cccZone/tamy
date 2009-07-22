#pragma once

#include "core-Renderer\AbstractGraphicalNode.h"


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalNodeMock : public AbstractGraphicalNode
{
public:
   AbstractGraphicalNodeMock(RenderingTechnique& technique)
         : AbstractGraphicalNode("AbstractGraphicalNodeMock", false, technique, 0)
   {}

   void render() {}
};

///////////////////////////////////////////////////////////////////////////////
