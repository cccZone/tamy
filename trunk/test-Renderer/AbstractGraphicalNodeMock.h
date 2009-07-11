#pragma once

#include "core-Renderer\AbstractGraphicalNode.h"


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalNodeMock : public AbstractGraphicalNode
{
public:
   AbstractGraphicalNodeMock(Material& material)
         : AbstractGraphicalNode("AbstractGraphicalNodeMock", false, material, 0)
   {}

   void render() {}
};

///////////////////////////////////////////////////////////////////////////////
