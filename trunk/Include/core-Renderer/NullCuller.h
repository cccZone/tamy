#pragma once

#include "core-Renderer\Culler.h"
#include "core\Frustum.h"


///////////////////////////////////////////////////////////////////////////////

class NullCuller : public Culler
{
private:
   Array<AbstractGraphicalNodeP>* m_renderingQueue;

public:
   void setup(Frustum& frustum,
              Array<AbstractGraphicalNodeP>& renderingQueue);

   void operator()(AbstractGraphicalNode& node);
};

///////////////////////////////////////////////////////////////////////////////
