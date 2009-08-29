#pragma once

#include "core-Renderer\RenderingProcessor.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingProcessorStub : public RenderingProcessor
{
public:
   void translate(Array<AbstractGraphicalNode*>& nodesToRender, 
                  RenderingTargetsPolicy& policy)
   {}
};

///////////////////////////////////////////////////////////////////////////////
